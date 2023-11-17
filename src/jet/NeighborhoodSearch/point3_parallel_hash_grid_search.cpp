#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/point_parallel_hash_grid_searcher3_generated.h>

#include <parallel.h>
#include <constants.h>
#include "point3_parallel_hash_grid_search.h"

#include <algorithm>
#include <vector>

namespace jet
{
    PointParallelHashGridSearch3::PointParallelHashGridSearch3
        (const Size3& resolution, double gridSpacing) :
            PointParallelHashGridSearch3(resolution.x, resolution.y, resolution.z, gridSpacing)
    {}

    PointParallelHashGridSearch3::PointParallelHashGridSearch3(size_t resolutionX,
                        size_t resolutionY, size_t resolutionZ, double gridSpacing)
                        : _GridSpacing(gridSpacing)
    {
        _Resolution.x = std::max(static_cast<ssize_t>(resolutionX), kOneSSize);
        _Resolution.y = std::max(static_cast<ssize_t>(resolutionY), kOneSSize);
        _Resolution.z = std::max(static_cast<ssize_t>(resolutionZ), kOneSSize);

        _StartIndexTable.resize(_Resolution.x * _Resolution.y * _Resolution.z, kMaxSize);
        _EndIndexTable.resize(_Resolution.x * _Resolution.y * _Resolution.z, kMaxSize);
    }


    PointParallelHashGridSearch3::PointParallelHashGridSearch3(const PointParallelHashGridSearch3& other)
    {
        Set(other);
    }

    void PointParallelHashGridSearch3::Build(const ConstArrayAccessor1<Vector3D>& points)
    {
        _Points.clear();
        _Keys.clear();
        _StartIndexTable.clear();
        _EndIndexTable.clear();
        _SortedIndices.clear();

        //Allocating memory chunk
        size_t NumPoints = points.Size();
        std::vector<size_t> TempKeys(NumPoints);
        _StartIndexTable.resize(_Resolution.x * _Resolution.y * _Resolution.z);
        _EndIndexTable.resize(_Resolution.x * _Resolution.y * _Resolution.z);
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

    void PointParallelHashGridSearch3::ForEachNearbyPoint(const Vector3D& origin,
                double radius, const ForEachNearbyPointCallback& callback) const
    {
        size_t NearbyKeys[8];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius* radius;

        for (int i = 0; i < 8; ++i)
        {
            size_t NearbyKey = NearbyKeys[i];
            size_t start = _StartIndexTable[NearbyKey];
            size_t end = _EndIndexTable[NearbyKey];

            if (start == kMaxSize)
                continue;

            for (size_t j = start; j < end; ++j)
            {
                Vector3D Direction = _Points[j] - origin;
                double DistanceSq = Direction.LengthSquared();
                if (DistanceSq <= QueryRadiusSq)
                {
                    double distance = 0.0;
                    if (DistanceSq > 0)
                    {
                        distance = std::sqrt(DistanceSq);
                        Direction /= distance;
                    }
                    callback(_SortedIndices[j], _Points[j]);

                }
            }
        }
    }

    bool PointParallelHashGridSearch3::HasNearbyPoint(const Vector3D& origin, double radius) const
    {
        size_t NearbyKeys[8];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for (int i = 0; i < 8; ++i)
        {
            size_t NearbyKey = NearbyKeys[i];
            size_t start = _StartIndexTable[NearbyKey];
            size_t end = _EndIndexTable[NearbyKey];

            // Empty Bucket - continue to next bucket
            if (start == kMaxSize)
                continue;

            for (size_t j = 0; j < end; ++j)
            {
                Vector3D direction = _Points[j] - origin;
                double DistanceSq = direction.LengthSquared();
                if (DistanceSq <= QueryRadiusSq)
                    return true;
            }
        }
        return false;
    }

    const std::vector<size_t>& PointParallelHashGridSearch3::Keys() const
    {
        return _Keys;
    }

    const std::vector<size_t>& PointParallelHashGridSearch3::StartIndexTable() const
    {
        return _StartIndexTable;
    }

    const std::vector<size_t>& PointParallelHashGridSearch3::EndIndexTable() const
    {
        return _EndIndexTable;
    }

    const std::vector<size_t>& PointParallelHashGridSearch3::SortedIndices() const
    {
        return _SortedIndices;
    }

    Point3I PointParallelHashGridSearch3::GetBucketIndex(const Vector3D& position) const
    {
        Point3I BucketIndex;
        BucketIndex.x = static_cast<ssize_t>(std::floor(position.x / _GridSpacing));
        BucketIndex.y = static_cast<ssize_t>(std::floor(position.y/ _GridSpacing));
        BucketIndex.z = static_cast<ssize_t>(std::floor(position.z/ _GridSpacing));
        return BucketIndex;
    }

    size_t PointParallelHashGridSearch3::GetHashKeyFromPosition(const Vector3D& position) const
    {
        Point3I BucketIndex = GetBucketIndex(position);
        return GetHashKeyFromBucketIndex(BucketIndex);
    }

    size_t PointParallelHashGridSearch3::GetHashKeyFromBucketIndex(const Point3I& BucketIndex) const
    {
        Point3I WrappedIndex = BucketIndex;
        WrappedIndex.x = BucketIndex.x % _Resolution.x;
        WrappedIndex.y = BucketIndex.y % _Resolution.y;
        WrappedIndex.z = BucketIndex.z % _Resolution.z;
        if (WrappedIndex.x < 0)
            WrappedIndex.x += _Resolution.x;

        if (WrappedIndex.y < 0)
            WrappedIndex.y += _Resolution.y;
        
        if (WrappedIndex.z < 0)
            WrappedIndex.z += _Resolution.z;

        return static_cast<size_t>((WrappedIndex.z * _Resolution.y + WrappedIndex.y) * _Resolution.x + WrappedIndex.x);
    }

    void PointParallelHashGridSearch3::GetNearbyKeys(const Vector3D& position, size_t* NearbyKeys) const
    {
        Point3I OriginIndex = GetBucketIndex(position), NearbyBucketIndices[8];

        for (int i = 0; i < 8; ++i)
        {
            NearbyBucketIndices[i] = OriginIndex;
        }

        if ((OriginIndex.x + 0.5f) * _GridSpacing <= position.x)
        {
            NearbyBucketIndices[4].x += 1;
            NearbyBucketIndices[5].x += 1;
            NearbyBucketIndices[6].x += 1;
            NearbyBucketIndices[7].x += 1;
        }
        else
        {
            NearbyBucketIndices[4].x -= 1;
            NearbyBucketIndices[5].x -= 1;
            NearbyBucketIndices[6].x -= 1;
            NearbyBucketIndices[7].x -= 1;
        }
        
        if ((OriginIndex.y + 0.5f) * _GridSpacing <= position.y)
        {
            NearbyBucketIndices[2].y += 1;
            NearbyBucketIndices[3].y += 1;
            NearbyBucketIndices[6].y += 1;
            NearbyBucketIndices[7].y += 1;
        }
        else
        {
            NearbyBucketIndices[2].y -= 1;
            NearbyBucketIndices[3].y -= 1;
            NearbyBucketIndices[6].y -= 1;
            NearbyBucketIndices[7].y -= 1;
        }
        
        if ((OriginIndex.z + 0.5f) * _GridSpacing <= position.z)
        {
            NearbyBucketIndices[1].z += 1;
            NearbyBucketIndices[3].z += 1;
            NearbyBucketIndices[5].z += 1;
            NearbyBucketIndices[7].z += 1;
        }
        else
        {
            NearbyBucketIndices[1].z -= 1;
            NearbyBucketIndices[3].z -= 1;
            NearbyBucketIndices[5].z -= 1;
            NearbyBucketIndices[7].z -= 1;
        }

        for (int i = 0; i < 8; ++i)
            NearbyKeys[i] = GetHashKeyFromBucketIndex(NearbyBucketIndices[i]);
    }

    PointNeighborSearch3Ptr PointParallelHashGridSearch3::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointParallelHashGridSearch3);
    }

    PointParallelHashGridSearch3& PointParallelHashGridSearch3::operator=(const PointParallelHashGridSearch3& other)
    {
        Set(other);
        return *this;
    }

    void PointParallelHashGridSearch3::Set(const PointParallelHashGridSearch3& other)
    {
        _GridSpacing = other._GridSpacing;
        _Resolution = other._Resolution;
        _Points = other._Points;
        _Keys = other._Keys;
        _StartIndexTable = other._StartIndexTable;
        _EndIndexTable = other._EndIndexTable;
        _SortedIndices = other._SortedIndices;
    }

    void PointParallelHashGridSearch3::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy Simple data
        auto fbsResolution = fbs::Size3(_Resolution.x, _Resolution.y, _Resolution.z);

        // Copy Points
        std::vector<fbs::Vector3D> points;
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
        auto fbsSearch = fbs::CreatePointParallelHashGridSearcher3(
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


    void PointParallelHashGridSearch3::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearch = fbs::GetPointParallelHashGridSearcher3(buffer.data());

        //Copy Simple Data
        auto res = FbsToJet(*fbsSearch->resolution());
        _Resolution.Set({res.x, res.y, res.z});
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

    PointParallelHashGridSearch3::Builder
    PointParallelHashGridSearch3::builder()
    {
        return Builder();
    }

    PointParallelHashGridSearch3::Builder&
    PointParallelHashGridSearch3::Builder::WithResolution(const Size3& resolution)
    {
        _Resolution = resolution;
        return *this;
    }

    PointParallelHashGridSearch3::Builder&
    PointParallelHashGridSearch3::Builder::WithGridSpacing(double GridSpacing)
    {
        _GridSpacing = GridSpacing;
        return *this;
    }

    PointParallelHashGridSearch3Ptr
    PointParallelHashGridSearch3::Builder::MakeShared() const
    {
        return std::shared_ptr<PointParallelHashGridSearch3>(new PointParallelHashGridSearch3(_Resolution, _GridSpacing),
                        [](PointParallelHashGridSearch3* obj){
                            delete obj;
                        });
    }

    PointNeighborSearch3Ptr
    PointParallelHashGridSearch3::Builder::BuildPointNeighborSearch() const
    {
        return MakeShared();
    }

}