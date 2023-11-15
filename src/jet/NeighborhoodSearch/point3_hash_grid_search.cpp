#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>

#include <IO/Serialization/generated/point_hash_grid_searcher3_generated.h>

#include <Arrays/array1.h>
#include "point3_hash_grid_search.h"

#include <algorithm>
#include <vector>

namespace jet
{
    PointHashGridSearch3::PointHashGridSearch3(const Size3& resolution, double gridSpacing)
                : PointHashGridSearch3(resolution.x, resolution.y, resolution.z, gridSpacing)
    {}

    PointHashGridSearch3::PointHashGridSearch3(size_t resolutionX, size_t resolutionY, size_t resolutionZ, double gridSpacing)
                : _GridSpacing(gridSpacing)
    { 
        _Resolution.x = std::max(static_cast<ssize_t>(resolutionX), kOneSSize);
        _Resolution.y = std::max(static_cast<ssize_t>(resolutionY), kOneSSize);
        _Resolution.z = std::max(static_cast<ssize_t>(resolutionZ), kOneSSize);
    }

    PointHashGridSearch3::PointHashGridSearch3(const PointHashGridSearch3& other)
    {
        Set(other);
    }

    void PointHashGridSearch3::Build(const ConstArrayAccessor1<Vector3D>& points)
    {
        _Buckets.clear();
        _Points.clear();

        //Allocating memory chunks.
        _Buckets.resize(_Resolution.x * _Resolution.y * _Resolution.z);
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

    void PointHashGridSearch3::ForEachNearbyPoint(const Vector3D& origin,
                        double radius, const ForEachNearbyPointCallback& callback) const
    {
        if (_Buckets.empty())
            return;

        size_t NearbyKeys[8];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for(int i = 0; i < 8; ++i)
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

    bool PointHashGridSearch3::HasNearbyPoint(const Vector3D& origin, double radius) const
    {
        if (_Buckets.empty())
            return false;

        size_t NearbyKeys[8];
        GetNearbyKeys(origin, NearbyKeys);

        const double QueryRadiusSq = radius * radius;

        for (int i = 0; i < 8; ++i)
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

    void PointHashGridSearch3::Add(const Vector3D& point)
    {
        if (_Buckets.empty())
        {
            Array1<Vector3D> arr = {point};
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
    PointHashGridSearch3::Buckets() const
    {
        return _Buckets;
    }

    Point3I PointHashGridSearch3::GetBucketIndex(const Vector3D& position) const
    {
        Point3I BucketIndex;
        BucketIndex.x = static_cast<ssize_t>(std::floor(position.x/_GridSpacing));

        BucketIndex.y = static_cast<ssize_t>(std::floor(position.y/ _GridSpacing));
        BucketIndex.z = static_cast<ssize_t>(std::floor(position.z/ _GridSpacing));
        return BucketIndex;
    }

    size_t PointHashGridSearch3::GetHashKeyFromPosition(const Vector3D& position) const
    {
        Point3I BucketIndex = GetBucketIndex(position);
        return GetHashKeyFromBucketIndex(BucketIndex);
    }

    size_t PointHashGridSearch3::GetHashKeyFromBucketIndex(const Point3I& BucketIndex) const
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

    void PointHashGridSearch3::GetNearbyKeys(const Vector3D& position, size_t* NearbyKeys) const
    {
        Point3I OriginIndex = GetBucketIndex(position), NearbyBucketIndices[8];

        for (int i =0; i< 8; ++i)
            NearbyBucketIndices[i] = OriginIndex;

        if ((OriginIndex.x + 0.5f) * _GridSpacing <= position.x){
            NearbyBucketIndices[4].x += 1;
            NearbyBucketIndices[5].x += 1;
            NearbyBucketIndices[6].x += 1;
            NearbyBucketIndices[7].x += 1;
        }
        else{
            NearbyBucketIndices[4].x -= 1;
            NearbyBucketIndices[5].x -= 1;
            NearbyBucketIndices[6].x -= 1;
            NearbyBucketIndices[7].x -= 1;
        }

        if ((OriginIndex.y + 0.5f) * _GridSpacing <= position.y){
            NearbyBucketIndices[2].y += 1;
            NearbyBucketIndices[3].y += 1;
            NearbyBucketIndices[6].y += 1;
            NearbyBucketIndices[7].y += 1;
        }
        else{
            NearbyBucketIndices[2].y -= 1;
            NearbyBucketIndices[3].y -= 1;
            NearbyBucketIndices[6].y -= 1;
            NearbyBucketIndices[7].y -= 1;
        }

        if ((OriginIndex.z + 0.5f) * _GridSpacing <= position.z){
            NearbyBucketIndices[1].z += 1;
            NearbyBucketIndices[3].z += 1;
            NearbyBucketIndices[5].z += 1;
            NearbyBucketIndices[7].z += 1;
        }
        else{
            NearbyBucketIndices[1].z -= 1;
            NearbyBucketIndices[3].z -= 1;
            NearbyBucketIndices[5].z -= 1;
            NearbyBucketIndices[7].z -= 1;
        }

        for (int i =0; i < 8; ++i)
            NearbyKeys[i] = GetHashKeyFromBucketIndex(NearbyBucketIndices[i]);
        
    }

    PointNeighborSearch3Ptr PointHashGridSearch3::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointHashGridSearch3);
    }

    PointHashGridSearch3& PointHashGridSearch3::operator=(const PointHashGridSearch3& other)
    {
        Set(other);
        return *this;
    }

    void PointHashGridSearch3::Set(const PointHashGridSearch3& other)
    {
        _GridSpacing = other._GridSpacing;
        _Resolution = other._Resolution;
        _Points = other._Points;
        _Buckets = other._Buckets;
    }

    void PointHashGridSearch3::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy Simple Data.
        auto fbsResolution = fbs::Size3(_Resolution.x, _Resolution.y, _Resolution.z);

        //Copy Points
        std::vector<fbs::Vector3D> points;
        for (const auto& pt: _Points)
            points.push_back(JetToFbs(pt));

        auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

        //Copy Buckets
        std::vector<flatbuffers::Offset<fbs::PointHashGridSearcherBucket3>> buckets;
        for (const auto& bucket: _Buckets)
        {
            std::vector<uint64_t> bucket64(bucket.begin(), bucket.end());
            flatbuffers::Offset<fbs::PointHashGridSearcherBucket3> fbsBucket = fbs::CreatePointHashGridSearcherBucket3(
                                                        builder, builder.CreateVector(bucket64.data(), bucket64.size()));
            
            buckets.push_back(fbsBucket);
        }

        auto fbsBuckets = builder.CreateVector(buckets);

        //Copy the searcher
        auto fbsSearcher = fbs::CreatePointHashGridSearcher3(builder, _GridSpacing, &fbsResolution, fbsPoints,
                                                fbsBuckets);

        builder.Finish(fbsSearcher);

        uint8_t *buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
        
    }

    void PointHashGridSearch3::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearcher = fbs::GetPointHashGridSearcher3(buffer.data());

        //Copy Simple Data.
        auto res = FbsToJet(*fbsSearcher->resolution());
        _Resolution.Set({res.x, res.y, res.z});
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

    PointHashGridSearch3::Builder PointHashGridSearch3::builder()
    {
        return Builder();
    }

    PointHashGridSearch3::Builder&
    PointHashGridSearch3::Builder::WithResolution(const Size3& resolution)
    {
        _Resolution = resolution;
        return *this;
    }
    
    PointHashGridSearch3::Builder&
    PointHashGridSearch3::Builder::WithGridSpacing(double GridSpacing)
    {
        _GridSpacing = GridSpacing;
        return *this;
    }

    PointHashGridSearch3
    PointHashGridSearch3::Builder::Build() const
    {
        return PointHashGridSearch3(_Resolution, _GridSpacing);
    }

    PointHashGridSearch3Ptr
    PointHashGridSearch3::Builder::MakeShared() const {
        return std::shared_ptr<PointHashGridSearch3>(
            new PointHashGridSearch3(_Resolution, _GridSpacing),
            [] (PointHashGridSearch3* obj) {
                delete obj;
            });
    }

    PointNeighborSearch3Ptr
    PointHashGridSearch3::Builder::BuildPointNeighborSearch() const {
        return MakeShared();
    }



}