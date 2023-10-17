#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>

#include <IO/Serialization/generated/point_hash_grid_search2_generated.h>

#include <Arrays/array1.h>
#include "point2_hash_grid_search.h"

#include <algorithm>
#include <vector>

namespace jet
{
    PointHashGridSearch2::PointHashGridSearch2(const Size2& resolution, double gridSpacing)
                : PointHashGridSearch2(resolution.x, resolution.y, gridSpacing)
    {}

    PointHashGridSearch2::PointHashGridSearch2(size_t resolutionX, size_t resolutionY, double gridSpacing)
                : _GridSpacing(gridSpacing)
    { 
        _Resolution.x = std::max(static_cast<ssize_t>(resolutionX), kOneSSize);
        _Resolution.y = std::max(static_cast<ssize_t>(resolutionY), kOneSSize);
    }

    PointHashGridSearch2::PointHashGridSearch2(const PointHashGridSearch2& other)
    {
        Set(other);
    }

    void PointHashGridSearch2::Build(const ConstArrayAccessor1<Vector2D>& points)
    {
        _Buckets.clear();
        _Points.clear();

        //Allocating memory chunks.
        _Buckets.resize(_Resolution.x * _Resolution.y);
        _Points.resize(points.Size());

        if (points.Size() == 0)
            return;
        
        //Put points into buckets
        for (size_t i = 0; i < points.Size(); ++i)
        {
            _Points[i] = points[i];
            size_t key = GetHashKeyFromPosition(points[i]);
            _Buckets[key].push_back(i);
        }
    }

    void PointHashGridSearch2::ForEachNearbyPoint(const Vector2D& origin,
                        double radius, const ForEachNearbyPointCallback& callback) const
    {
        if (_Buckets.empty())
            return;

        size_t NearbyKeys[4];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for(int i = 0; i < 4; ++i)
        {
            const auto& bucket = _Buckets[NearbyKeys[i]];
            size_t NumberOfPointsInBucket = bucket.size();

            for (size_t j = 0; j < NumberOfPointsInBucket; ++j)
            {
                size_t PointIndex = bucket[j];
                double rSquared = (_Points[PointIndex] - origin).LengthSquared();
                if (rSquared <= QueryRadiusSq)
                {
                    callback(PointIndex, _Points[PointIndex]);
                }
            }
        }
    }

    bool PointHashGridSearch2::HasNearbyPoint(const Vector2D& origin, double radius) const
    {
        if (_Buckets.empty())
            return false;

        size_t NearbyKeys[4];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for (int i = 0; i < 4; ++i)
        {
            const auto& bucket = _Buckets[NearbyKeys[i]];
            size_t NumberOfPointsInBucket = bucket.size();

            for (size_t j = 0; j < NumberOfPointsInBucket; ++j)
            {
                size_t PointIndex = bucket[j];
                double rSquared = (_Points[PointIndex] - origin).LengthSquared();
                if(rSquared <= QueryRadiusSq)
                    return true;
            }
        }

        return false;
    }

    void PointHashGridSearch2::Add(const Vector2D& point)
    {
        if (_Buckets.empty())
        {
            Array1<Vector2D> arr = {point};
            Build(arr);
        }
        else
        {
            size_t i = _Points.size();
            _Points.push_back(point);
            size_t key = GetHashKeyFromPosition(point);
            _Buckets[key].push_back(i);
        }
    }

    const std::vector<std::vector<size_t>>&
    PointHashGridSearch2::Buckets() const
    {
        return _Buckets;
    }

    Point2I PointHashGridSearch2::GetBucketIndex(const Vector2D& position) const
    {
        Point2I BucketIndex;
        BucketIndex.x = static_cast<ssize_t>(std::floor(position.x/_GridSpacing));

        BucketIndex.y = static_cast<ssize_t>(std::floor(position.y/ _GridSpacing));
        return BucketIndex;
    }

    size_t PointHashGridSearch2::GetHashKeyFromPosition(const Vector2D& position) const
    {
        Point2I BucketIndex = GetBucketIndex(position);
        return GetHashKeyFromBucketIndex(BucketIndex);
    }

    size_t PointHashGridSearch2::GetHashKeyFromBucketIndex(const Point2I& BucketIndex) const
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

    void PointHashGridSearch2::GetNearbyKeys(const Vector2D& position, size_t* NearbyKeys) const
    {
        Point2I OriginIndex = GetBucketIndex(position), NearbyBucketIndices[4];

        for (int i =0; i< 4; ++i)
            NearbyBucketIndices[i] = OriginIndex;

        if ((OriginIndex.x + 0.5f) * _GridSpacing <= position.x){
            NearbyBucketIndices[2].x += 1;
            NearbyBucketIndices[3].x += 1;
        }
        else{
            NearbyBucketIndices[2].x -= 1;
            NearbyBucketIndices[3].x -= 1;
        }

        if ((OriginIndex.y + 0.5f) * _GridSpacing <= position.y){
            NearbyBucketIndices[1].y += 1;
            NearbyBucketIndices[3].y += 1;
        }
        else{
            NearbyBucketIndices[1].y -= 1;
            NearbyBucketIndices[3].y -= 1;
        }

        for (int i =0; i < 4; ++i)
            NearbyKeys[i] = GetHashKeyFromBucketIndex(NearbyBucketIndices[i]);
        
    }

    PointNeighborSearch2Ptr PointHashGridSearch2::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointHashGridSearch2);
    }

    PointHashGridSearch2& PointHashGridSearch2::operator=(const PointHashGridSearch2& other)
    {
        Set(other);
        return *this;
    }

    void PointHashGridSearch2::Set(const PointHashGridSearch2& other)
    {
        _GridSpacing = other._GridSpacing;
        _Resolution = other._Resolution;
        _Points = other._Points;
        _Buckets = other._Buckets;
    }

    void PointHashGridSearch2::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy Simple Data.
        auto fbsResolution = fbs::Size2(_Resolution.x, _Resolution.y);

        //Copy Points
        std::vector<fbs::Vector2D> points;
        for (const auto& pt: _Points)
            points.push_back(JetToFbs(pt));

        auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

        //Copy Buckets
        std::vector<flatbuffers::Offset<fbs::PointHashGridSearchBucket2>> buckets;
        for (const auto& bucket: _Buckets)
        {
            std::vector<uint64_t> bucket64(bucket.begin(), bucket.end());
            flatbuffers::Offset<fbs::PointHashGridSearchBucket2> fbsBucket = fbs::CreatePointHashGridSearchBucket2(
                                                        builder, builder.CreateVector(bucket64.data(), bucket64.size()));
            
            buckets.push_back(fbsBucket);
        }

        auto fbsBuckets = builder.CreateVector(buckets);

        //Copy the searcher
        auto fbsSearcher = fbs::CreatePointHashGridSearch2(builder, _GridSpacing, &fbsResolution, fbsPoints,
                                                fbsBuckets);

        builder.Finish(fbsSearcher);

        uint8_t *buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
        
    }

    void PointHashGridSearch2::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearcher = fbs::GetPointHashGridSearch2(buffer.data());

        //Copy Simple Data.
        auto res = FbsToJet(*fbsSearcher->resolution());
        _Resolution.Set({res.x, res.y});
        _GridSpacing = fbsSearcher->gridSpacing();

        //Copy Points
        auto fbsPoints = fbsSearcher->points();
        _Points.resize(fbsPoints->size());
        for(uint32_t i =0; i< fbsPoints->size(); ++i)
            _Points[i] = FbsToJet(*fbsPoints->Get(i));

        //Copy Buckets
        auto fbsBuckets = fbsSearcher->buckets();
        _Buckets.resize(fbsBuckets->size());
        for (uint32_t i = 0; i< fbsBuckets->size(); ++i)
        {
            auto fbsBucket = fbsBuckets->Get(i);
            _Buckets[i].resize(fbsBucket->data()->size());
            std::transform(fbsBucket->data()->begin(),
                        fbsBucket->data()->end(),
                        _Buckets[i].begin(),
                        [](uint64_t val){
                            return static_cast<size_t>(val);
                        });
        }
    }

    PointHashGridSearch2::Builder PointHashGridSearch2::builder()
    {
        return Builder();
    }

    PointHashGridSearch2::Builder&
    PointHashGridSearch2::Builder::WithResolution(const Size2& resolution)
    {
        _Resolution = resolution;
        return *this;
    }
    
    PointHashGridSearch2::Builder&
    PointHashGridSearch2::Builder::WithGridSpacing(double GridSpacing)
    {
        _GridSpacing = GridSpacing;
        return *this;
    }

    PointHashGridSearch2
    PointHashGridSearch2::Builder::Build() const
    {
        return PointHashGridSearch2(_Resolution, _GridSpacing);
    }

    PointHashGridSearch2Ptr
    PointHashGridSearch2::Builder::MakeShared() const {
        return std::shared_ptr<PointHashGridSearch2>(
            new PointHashGridSearch2(_Resolution, _GridSpacing),
            [] (PointHashGridSearch2* obj) {
                delete obj;
            });
    }

    PointNeighborSearch2Ptr
    PointHashGridSearch2::Builder::BuildPointNeighborSearch() const {
        return MakeShared();
    }



}