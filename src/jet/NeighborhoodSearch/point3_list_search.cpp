#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/point_simple_list_searcher3_generated.h>

#include "point3_list_search.h"
#include <algorithm>
#include <vector>

namespace jet
{
    PointListSearch3::PointListSearch3()
    {}

    PointListSearch3::PointListSearch3(const PointListSearch3& other)
    {
        Set(other);
    }

    void PointListSearch3::Build(const ConstArrayAccessor1<Vector3D>& points)
    {
        _Points.resize(points.Size());
        std::copy(points.Data(), points.Data() + points.Size(), _Points.begin());
    }

    void PointListSearch3::ForEachNearbyPoint(const Vector3D& origin,
                        double radius, const std::function<void(size_t, const Vector3D&)>& callback) const
    {
        double RadiusSq = radius * radius;
        for (size_t i = 0; i < _Points.size(); ++i)
        {
            Vector3D r = _Points[i] - origin;
            double DistanceSq = r.Dot(r);
            if (DistanceSq <= RadiusSq)
                callback(i, _Points[i]);
        }
    }

    bool PointListSearch3::HasNearbyPoint(const Vector3D& origin, double radius) const
    {
        double RadiusSq = radius * radius;
        for (size_t i = 0; i < _Points.size(); ++i)
        {
            Vector3D r = _Points[i] - origin;
            double DistanceSq = r.Dot(r);
            if (DistanceSq <= RadiusSq)
                return true;
        }

        return false;
    }

    PointNeighborSearch3Ptr PointListSearch3::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointListSearch3);
    }

    PointListSearch3& PointListSearch3::operator=(const PointListSearch3& other)
    {
        Set(other);
        return *this;
    }

    void PointListSearch3::Set(const PointListSearch3& other)
    {
        _Points = other._Points;
    }

    void PointListSearch3::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy points
        std::vector<fbs::Vector3D> points;
        for (const auto& pt: _Points)
            points.push_back(JetToFbs(pt));
        
        auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

        //Copy the search
        auto fbsSearch = fbs::CreatePointSimpleListSearcher3(builder, fbsPoints);

        builder.Finish(fbsSearch);

        uint8_t * buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
    }

    void PointListSearch3::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearch = fbs::GetPointSimpleListSearcher3(buffer.data());

        //Copy Points.
        auto fbsPoints = fbsSearch->points();
        _Points.resize(fbsPoints->size());
        for (uint32_t i = 0; i < fbsPoints->size(); ++i)
        {
            _Points[i] = FbsToJet(*fbsPoints->Get(i));
        }
    }

    PointListSearch3 PointListSearch3::Builder::Build() const
    {
        return PointListSearch3();
    }

    PointListSearch3Ptr PointListSearch3::Builder::MakeShared() const
    {
        return std::shared_ptr<PointListSearch3>(
                new PointListSearch3(),
                [](PointListSearch3* obj)
                {
                    delete obj;
                });
    }

    PointNeighborSearch3Ptr PointListSearch3::Builder::BuildPointNeighborSearch() const
    {
        return MakeShared();
    }
}