#include <jet.h>
#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/point_simple_list_searcher2_generated.h>

#include "point2_list search.h"

#include <algorithm>
#include <vector>

namespace jet
{
    PointListSearch2::PointListSearch2()
    {}

    PointListSearch2::PointListSearch2(const PointListSearch2& other)
    {
        Set(other);
    }


    void PointListSearch2::Build(const ConstArrayAccessor1<Vector2D>& points)
    {
        _Points.resize(points.Size());
        std::copy(points.Data(), points.Data() + points.Size(), _Points.begin());
    }

    void PointListSearch2::ForEachNearbyPoint(const Vector2D& origin,
                    double radius, const ForEachNearbyPointCallback& callback) const
    {
        double RadiusSq = radius * radius;
        for (size_t i = 0; i < _Points.size(); ++i)
        {
            Vector2D r = _Points[i] - origin;
            double DistanceSq = r.Dot(r);
            if (DistanceSq <= RadiusSq)
                callback(i, _Points[i]);
        }
    }

    bool PointListSearch2::HasNearbyPoint(const Vector2D& origin, double radius) const
    {
        double RadiusSq = radius * radius;
        for (size_t i = 0; i < _Points.size(); ++i)
        {
            Vector2D r = _Points[i] - origin;
            double DistanceSq = r.Dot(r);
            if (DistanceSq <= RadiusSq)
                return true;
        }
        return false;
    }

    PointNeighborSearch2Ptr PointListSearch2::Clone() const
    {
        return CLONE_W_CUSTOM_DELETER(PointListSearch2);
    }

    PointListSearch2& PointListSearch2::operator=(const PointListSearch2& other)
    {
        Set(other);
        return *this;
    }

    void PointListSearch2::Set(const PointListSearch2& other)
    {
        _Points = other._Points;
    }

    void PointListSearch2::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        //Copy Points
        std::vector<fbs::Vector2D> points;
        for (const auto& pt: _Points)
            points.push_back(JetToFbs(pt));

        auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());

        //Copy the Search class.
        auto fbsSearcher = fbs::CreatePointSimpleListSearcher2(builder, fbsPoints);

        builder.Finish(fbsSearcher);

        uint8_t *buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();

        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
    }

    void PointListSearch2::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsSearcher = fbs::GetPointSimpleListSearcher2(buffer.data());

        //Copy Points
        auto fbsPoints = fbsSearcher->points();
        _Points.resize(fbsPoints->size());
        for (uint32_t i = 0; i < fbsPoints->size(); ++i)
            _Points[i] = FbsToJet(*fbsPoints->Get(i));
    }

    PointListSearch2 PointListSearch2::Builder::Build() const
    {
        return PointListSearch2();
    }

    PointListSearch2Ptr PointListSearch2::Builder::MakeShared() const
    {
        return std::shared_ptr<PointListSearch2>(
                new PointListSearch2(),
                [](PointListSearch2* obj){
                    delete obj;
        });
    }

    PointNeighborSearch2Ptr PointListSearch2::Builder::BuildPointNeighborSearch() const
    {
        return MakeShared();
    }
}