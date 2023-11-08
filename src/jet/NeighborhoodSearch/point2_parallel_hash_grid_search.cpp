#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/point_parallel_hash_grid_searcher2_generated.h>

#include <parallel.h>
#include <constants.h>
#include "point2_parallel_hash_grid_search.h"

#include <algorithm>
#include <vector>

namespace jet
{
    PointParallelHashGridSearch2::PointParallelHashGridSearch2
        (const Size2& resolution, double gridSpacing) :
            PointParallelHashGridSearch2(resolution.x, resolution.y, gridSpacing)
    {}

    PointParallelHashGridSearch2::PointParallelHashGridSearch2(size_t resolutionX,
                        size_t resolutionY, double gridSpacing)
                        : _GridSpacing(gridSpacing)
    {
        _Resolution.x = std::max(static_cast<ssize_t>(resolutionX), kOneSSize);
        _Resolution.y = std::max(static_cast<ssize_t>(resolutionY), kOneSSize);

        _StartIndexTable.resize(_Resolution.x * _Resolution.y, kMaxSize);
        _EndIndexTable.resize(_Resolution.x * _Resolution.y, kMaxSize);
    }


    PointParallelHashGridSearch2::PointParallelHashGridSearch2(const PointParallelHashGridSearch2& other)
    {
        Set(other);
    }

    void PointParallelHashGridSearch2::Build(const ConstArrayAccessor1<Vector2D>& points)
    {
        _Points.clear();
        _Keys.clear();
        _StartIndexTable.clear();
        _EndIndexTable.clear();
        _SortedIndices.clear();

        //Allocating memory chunk
        size_t NumPoints = points.Size();
        std::vector<size_t> TempKeys(NumPoints);
        _StartIndexTable.resize(_Resolution.x * _Resolution.y);
        _EndIndexTable.resize(_Resolution.x * _Resolution.y);
        ParallelFill(_StartIndexTable.begin(), _StartIndexTable.end(), kMaxSize);
        ParallelFill(_EndIndexTable.begin(), _EndIndexTable.end(), kMaxSize);
        _Keys.resize(NumPoints);
        _SortedIndices.resize(NumPoints);
        _Points.resize(NumPoints);

        if (NumPoints == 0)
            return;
        
        //Initialize indices array and generate hash key for each point.
        ParallelFor(kZeroSize, NumPoints, [&](size_t i){
            _SortedIndices[i] = i;
            _Points[i] = points[i];
            TempKeys[i] = GetHashKeyFromPosition(points[i]);
        });

        //Sort Indices based on hash key.
        ParallelSort(
            _SortedIndices.begin(),
            _SortedIndices.end(),
            [&TempKeys](size_t indexA, size_t indexB)
            {
                return TempKeys[indexA] < TempKeys[indexB];
            });

        //Reorder point and key arrays.
        ParallelFor(kZeroSize, NumPoints,
                [&](size_t i){
                    _Points[i] = points[_SortedIndices[i]];
                    _Keys[i] = TempKeys[_SortedIndices[i]];
                });

        // The _Points and _Keys are sorted by points' hash key values.
        //Now filling start and end index table with _Keys.

        // Assume that _Keys array looks like this:
        // [5| 8| 8| 10| 10 ]
        // Then _Start Index Table and _EndIndexTable should look like:
        // [.....|0|...|1|..|3|..]
        // [.....|1|...|3|..|6|..]
        //       ^5    ^8   ^10
        // such that _EndIndexTable[i] - _StartIndexTable[i] is the number of points
        // in i-th table bucket.
        _StartIndexTable[_Keys[0]] = 0;
        _EndIndexTable[_Keys[NumPoints-1]] = NumPoints;

        ParallelFor((size_t) 1, NumPoints,
                [&](size_t i){
                    if (_Keys[i] > _Keys[i-1])
                    {
                        _StartIndexTable[_Keys[i]] = i;
                        _EndIndexTable[_Keys[i - 1]] = i;
                    }
                });

        size_t SumNumPointsPerBucket = 0;
        size_t MaxNumPointsPerBucket = 0;
        size_t NumNonEmptyBucket = 0;
        for(size_t i = 0; i < _StartIndexTable.size(); ++i)
        {
            if (_StartIndexTable[i] != kMaxSize)
            {
                size_t NumPointsInBucket = _EndIndexTable[i] - _StartIndexTable[i];
                SumNumPointsPerBucket += NumPointsInBucket;
                MaxNumPointsPerBucket = std::max(MaxNumPointsPerBucket, NumPointsInBucket);
                ++NumNonEmptyBucket;
            }
        }

        JET_INFO << "Avg. Number of Points per Non-Empty Bucket: "
                    << static_cast<float>(SumNumPointsPerBucket) / static_cast<float>(NumNonEmptyBucket);
        JET_INFO << "Max Number of Points in a bucket: "
            << MaxNumPointsPerBucket;
    }

    void PointParallelHashGridSearch2::ForEachNearbyPoint(const Vector2D& origin,
                double radius, const ForEachNearbyPointCallback& callback) const
    {
        size_t NearbyKeys[4];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius* radius;

        for (int i = 0; i < 4; ++i)
        {
            size_t NearbyKey = NearbyKeys[i];
            size_t start = _StartIndexTable[NearbyKey];
            size_t end = _EndIndexTable[NearbyKey];

            if (start == kMaxSize)
                continue;

            for (size_t j = start; j < end; ++j)
            {
                Vector2D Direction = _Points[j] - origin;
                double DistanceSq = Direction.LengthSquared();
                if (DistanceSq <= QueryRadiusSq)
                    callback(_SortedIndices[j], _Points[j]);
            }
        }
    }

    bool PointParallelHashGridSearch2::HasNearbyPoint(const Vector2D& origin, double radius) const
    {
        size_t NearbyKeys[4];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for (int i = 0; i < 4; ++i)
        {
            size_t NearbyKey = NearbyKeys[i];
            size_t start = _StartIndexTable[NearbyKey];
            size_t end = _EndIndexTable[NearbyKey];

            // Empty Bucket - continue to next bucket
            if (start == kMaxSize)
                continue;

            for (size_t j = 0; j < end; ++j)
            {
                Vector2D direction = _Points[j] - origin;
                double DistanceSq = direction.LengthSquared();
                if (DistanceSq <= QueryRadiusSq)
                    return true;
            }
        }
        return false;
    }

    const std::vector<size_t>& PointParallelHashGridSearch2::Keys() const
    {
        return _Keys;
    }

    const std::vector<size_t>& PointParallelHashGridSearch2::StartIndexTable() const
    {
        return _StartIndexTable;
    }

    const std::vector<size_t>& PointParallelHashGridSearch2::EndIndexTable() const
    {
        return _EndIndexTable;
    }

    const std::vector<size_t>& PointParallelHashGridSearch2::SortedIndices() const
    {
        return _SortedIndices;
    }

    Point2I PointParallelHashGridSearch2::GetBucketIndex(const Vector2D& position) const
    {
        Point2I BucketIndex;
        BucketIndex.x = static_cast<ssize_t>(std::floor(position.x / _GridSpacing));
        BucketIndex.y = static_cast<ssize_t>(std::floor(position.y/ _GridSpacing));
        return BucketIndex;
    }

    size_t PointParallelHashGridSearch2::GetHashKeyFromPosition(const Vector2D& position) const
    {
        Point2I BucketIndex = GetBucketIndex(position);
        return GetHashKeyFromBucketIndex(BucketIndex);
    }

    size_t PointParallelHashGridSearch2::GetHashKeyFromBucketIndex(const Point2I& BucketIndex) const
    {
        Point2I WrappedIndex = BucketIndex;
        WrappedIndex.x = BucketIndex.x % _Resolution.x;
        WrappedIndex.y = BucketIndex.y % _Resolution.y;
        if (WrappedIndex.x < 0)
            WrappedIndex.x += _Resolution.x;

        if (WrappedIndex.y < 0)
            WrappedIndex.y += _Resolution.y;

        return static_cast<size_t>(WrappedIndex.y * _Resolution.x + WrappedIndex.x);
    }

    void PointParallelHashGridSearch2::GetNearbyKeys(const Vector2D& position, size_t* NearbyKeys) const
    {
        Point2I OriginIndex = GetBucketIndex(position), NearbyBucketIndices[4];

        for (int i = 0; i < 4; ++i)
        {
            NearbyBucketIndices[i] = OriginIndex;
        }

        if ((OriginIndex.x + 0.5f) * _GridSpacing <= position.x)
        {
            NearbyBucketIndices[2].x += 1;
            NearbyBucketIndices[3].x += 1;
        }
        else
        {
            NearbyBucketIndices[2].x -= 1;
            NearbyBucketIndices[3].x -= 1;
        }
        
        if ((OriginIndex.y + 0.5f) * _GridSpacing <= position.y)
        {
            NearbyBucketIndices[1].y += 1;
            NearbyBucketIndices[3].y += 1;
        }
        else
        {
            NearbyBucketIndices[1].y -= 1;
            NearbyBucketIndices[3].y -= 1;
        }

        for (int i = 0; i < 4; ++i)
            NearbyKeys[i] = GetHashKeyFromBucketIndex(NearbyBucketIndices[i]);
    }

    PointNeighborSearch2Ptr PointParallelHashGridSearch2::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointParallelHashGridSearch2);
    }

    PointParallelHashGridSearch2& PointParallelHashGridSearch2::operator=(const PointParallelHashGridSearch2& other)
    {
        Set(other);
        return *this;
    }

    void PointParallelHashGridSearch2::Set(const PointParallelHashGridSearch2& other)
    {
        _GridSpacing = other._GridSpacing;
        _Resolution = other._Resolution;
        _Points = other._Points;
        _Keys = other._Keys;
        _StartIndexTable = other._StartIndexTable;
        _EndIndexTable = other._EndIndexTable;
        _SortedIndices = other._SortedIndices;
    }

    void PointParallelHashGridSearch2::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy Simple data
        auto fbsResolution = fbs::Size2(_Resolution.x, _Resolution.y);

        // Copy Points
        std::vector<fbs::Vector2D> points;
        for (const auto& pt: _Points)
            points.push_back(JetToFbs(pt));

        auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

        //Copy Key Tables
        std::vector<uint64_t> keys(_Keys.begin(), _Keys.end());
        std::vector<uint64_t> startIndexTable(_StartIndexTable.begin(), _StartIndexTable.end());
        std::vector<uint64_t> endIndexTable(_EndIndexTable.begin(),_EndIndexTable.end());
        std::vector<uint64_t> sortedIndices(_SortedIndices.begin(), _SortedIndices.end());

        auto fbsKeys = builder.CreateVector(keys.data(), keys.size());
        auto fbsStartIndexTable = builder.CreateVector(startIndexTable.data(), startIndexTable.size());
        auto fbsEndIndexTable = builder.CreateVector(endIndexTable.data(), endIndexTable.size());
        auto fbsSortedIndices = builder.CreateVector(sortedIndices.data(), sortedIndices.size());

        //Copy the Search instance.
        auto fbsSearch = fbs::CreatePointParallelHashGridSearcher2(
                            builder,
                            _GridSpacing,
                            &fbsResolution,
                            fbsPoints,
                            fbsKeys,
                            fbsStartIndexTable,
                            fbsEndIndexTable,
                            fbsSortedIndices);
        
        builder.Finish(fbsSearch);

        uint8_t * buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
    }


    void PointParallelHashGridSearch2::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearch = fbs::GetPointParallelHashGridSearcher2(buffer.data());

        //Copy Simple Data
        auto res = FbsToJet(*fbsSearch->resolution());
        _Resolution.Set({res.x, res.y});
        _GridSpacing = fbsSearch->gridSpacing();

        //Copy points
        auto fbsPoints = fbsSearch->points();
        _Points.resize(fbsPoints->size());
        for (uint32_t i = 0; i < fbsPoints->size(); ++i)
            _Points[i] = FbsToJet(*fbsPoints->Get(i));

        //Copy key / tables
        auto fbsKeys = fbsSearch->keys();
        _Keys.resize(fbsKeys->size());
        for (uint32_t i =0; i < fbsKeys->size(); ++i)
            _Keys[i] = static_cast<size_t>(fbsKeys->Get(i));
        
        auto fbsStartIndexTable = fbsSearch->startIndexTable();
        _StartIndexTable.resize(fbsStartIndexTable->size());
        for (uint32_t i =0; i < fbsStartIndexTable->size(); ++i)
            _StartIndexTable[i] = static_cast<size_t>(fbsStartIndexTable->Get(i));
        
        auto fbsEndIndexTable = fbsSearch->endIndexTable();
        _EndIndexTable.resize(fbsEndIndexTable->size());
        for (uint32_t i =0; i < fbsEndIndexTable->size(); ++i)
            _EndIndexTable[i] = static_cast<size_t>(fbsEndIndexTable->Get(i));
        
        
        auto fbsSortedIndices = fbsSearch->sortedIndices();
        _SortedIndices.resize(fbsSortedIndices->size());
        for (uint32_t i =0; i < fbsSortedIndices->size(); ++i)
            _SortedIndices[i] = static_cast<size_t>(fbsSortedIndices->Get(i));
    }

    PointParallelHashGridSearch2::Builder
    PointParallelHashGridSearch2::builder()
    {
        return Builder();
    }

    PointParallelHashGridSearch2::Builder&
    PointParallelHashGridSearch2::Builder::WithResolution(const Size2& resolution)
    {
        _Resolution = resolution;
        return *this;
    }

    PointParallelHashGridSearch2::Builder&
    PointParallelHashGridSearch2::Builder::WithGridSpacing(double GridSpacing)
    {
        _GridSpacing = GridSpacing;
        return *this;
    }

    PointParallelHashGridSearch2Ptr
    PointParallelHashGridSearch2::Builder::MakeShared() const
    {
        return std::shared_ptr<PointParallelHashGridSearch2>(new PointParallelHashGridSearch2(_Resolution, _GridSpacing),
                        [](PointParallelHashGridSearch2* obj){
                            delete obj;
                        });
    }

    PointNeighborSearch2Ptr
    PointParallelHashGridSearch2::Builder::BuildPointNeighborSearch() const
    {
        return MakeShared();
    }

}