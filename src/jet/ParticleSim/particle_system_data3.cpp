#include <jet.h>
#include <macros.h>
#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/particle_system_data3_generated.h>
#include <IO/Serialization/factory.h>

#include <parallel.h>
#include "particle_system_data3.h"
#include <NeighborhoodSearch/point3_parallel_hash_grid_search.h>
#include <timer.h>

#include <algorithm>
#include <vector>

namespace jet
{
    static const size_t kDefaultHashGridResolution = 64;

    ParticleSystemData3::ParticleSystemData3()
            : ParticleSystemData3(0)
    {}

    ParticleSystemData3::ParticleSystemData3(size_t NumberOfParticles)
    {
        _PositionIdx = AddVectorData();
        _VelocityIdx = AddVectorData();
        _ForceIdx = AddVectorData();

        // Use PointParallelHashGridSearch3 by default
        _NeighborSearch = std::make_shared<PointParallelHashGridSearch3>
                            (kDefaultHashGridResolution, kDefaultHashGridResolution,
                            kDefaultHashGridResolution ,2.0* _Radius);

        Resize(NumberOfParticles);
    }

    ParticleSystemData3::ParticleSystemData3(const ParticleSystemData3& other)
    {
        Set(other);
    }

    ParticleSystemData3::~ParticleSystemData3()
    {}


    void ParticleSystemData3::Resize(size_t NewNumberOfPoints)
    {
        _NumberOfParticles = NewNumberOfPoints;

        for (auto& attr: _ScalarDataList)
            attr.Resize(NewNumberOfPoints, 0.0);

        for (auto& attr: _VectorDataList)
            attr.Resize(NewNumberOfPoints, Vector3D());
    }

    size_t ParticleSystemData3::NumberOfParticles() const
    {
        return _NumberOfParticles;
    }

    size_t ParticleSystemData3::AddScalarData(double InitVal)
    {
        size_t attrIdx = _ScalarDataList.size();
        _ScalarDataList.emplace_back(NumberOfParticles(), InitVal);
        return attrIdx;
    }

    size_t ParticleSystemData3::AddVectorData(const Vector3D& InitVal)
    {
        size_t attrIdx = _VectorDataList.size();
        _VectorDataList.emplace_back(NumberOfParticles(), InitVal);
        return attrIdx;
    }

    double ParticleSystemData3::Radius() const
    {
        return _Radius;
    }

    void ParticleSystemData3::SetRadius(double NewRadius)
    {
        _Radius = std::max(NewRadius, 0.0);
    }

    double ParticleSystemData3::Mass() const
    {
        return _Mass;
    }

    void ParticleSystemData3::SetMass(double NewMass)
    {
        _Mass = std::max(NewMass, 0.0);
    }

    ConstArrayAccessor1<Vector3D> ParticleSystemData3::Positions() const
    {
        return VectorDataAt(_PositionIdx);
    }

    ArrayAccessor1<Vector3D> ParticleSystemData3::Positions()
    {
        return VectorDataAt(_PositionIdx);
    }

    ConstArrayAccessor1<Vector3D> ParticleSystemData3::Velocities() const
    {
        return VectorDataAt(_VelocityIdx);
    }

    ArrayAccessor1<Vector3D> ParticleSystemData3::Velocities()
    {
        return VectorDataAt(_VelocityIdx);
    }

    ConstArrayAccessor1<Vector3D> ParticleSystemData3::Forces() const
    {
        return VectorDataAt(_ForceIdx);
    }

    ArrayAccessor1<Vector3D> ParticleSystemData3::Forces()
    {
        return VectorDataAt(_ForceIdx);
    }

    ConstArrayAccessor1<double> ParticleSystemData3::ScalarDataAt(size_t idx) const
    {
        return _ScalarDataList[idx].ConstAccessor();
    }

    ArrayAccessor1<double> ParticleSystemData3::ScalarDataAt(size_t idx)
    {
        return _ScalarDataList[idx].Accessor();
    }

    ConstArrayAccessor1<Vector3D> ParticleSystemData3::VectorDataAt(size_t idx) const
    {
        return _VectorDataList[idx].ConstAccessor();
    }

    ArrayAccessor1<Vector3D> ParticleSystemData3::VectorDataAt(size_t idx)
    {
        return _VectorDataList[idx].Accessor();
    }

    void ParticleSystemData3::AddParticle(const Vector3D& NewPosition,
                    const Vector3D& NewVelocity, const Vector3D& NewForce)
    {
        Array1<Vector3D> NewPositions = {NewPosition};
        Array1<Vector3D> NewVelocities = {NewVelocity};
        Array1<Vector3D> NewForces = {NewForce};

        AddParticles(NewPositions.ConstAccessor(), NewVelocities.ConstAccessor(), NewForces.ConstAccessor());
    }

    void ParticleSystemData3::AddParticles(const ConstArrayAccessor1<Vector3D>& NewPositions,
                            const ConstArrayAccessor1<Vector3D>& NewVelocities,
                            const ConstArrayAccessor1<Vector3D>& NewForces)
    {
        JET_THROW_INVALID_ARG_IF(NewVelocities.Size() > 0 && NewVelocities.Size()!= NewPositions.Size());
        JET_THROW_INVALID_ARG_IF(NewForces.Size() > 0 && NewForces.Size()!= NewPositions.Size());

        size_t OldNumberOfParticles = NumberOfParticles();
        size_t NewNumberOfParticles = OldNumberOfParticles + NewPositions.Size();

        Resize(NewNumberOfParticles);

        auto Pos = Positions();
        auto Vel = Velocities();
        auto Force = Forces();

        ParallelFor(kZeroSize, NewPositions.Size(),
                [&](size_t i){
                     Pos[i + OldNumberOfParticles] = NewPositions[i];
                });

        if (NewVelocities.Size() > 0)
        {
            ParallelFor(kZeroSize, NewPositions.Size(),
                [&](size_t i){
                     Vel[i + OldNumberOfParticles] = NewVelocities[i];
                });    
        }

        if (NewForces.Size() > 0)
        {
            ParallelFor(kZeroSize, NewPositions.Size(),
                [&](size_t i){
                     Force[i + OldNumberOfParticles] = NewForces[i];
                });    
        }
        
    }

    const PointNeighborSearch3Ptr& ParticleSystemData3::NeighborSearch() const
    {
        return _NeighborSearch;
    }

    void ParticleSystemData3::SetNeighborSearch(const PointNeighborSearch3Ptr& NewNeighborSearch)
    {
        _NeighborSearch = NewNeighborSearch;
    }

    const std::vector<std::vector<size_t>>& ParticleSystemData3::NeighborLists() const
    {
        return _NeighborLists;
    }

    void ParticleSystemData3::BuildNeighborSearch(double MaxSearchRadius)
    {
        Timer timer;

        //Using PointParallelHashGridSearch3 by default.
        _NeighborSearch = std::make_shared<PointParallelHashGridSearch3>(
                    kDefaultHashGridResolution, kDefaultHashGridResolution, kDefaultHashGridResolution,
                    2.0 * MaxSearchRadius);
        _NeighborSearch->Build(Positions());

        JET_INFO << "Building Neighbor Search took: "
                << timer.DurationInSeconds()
                << " seconds";
    }

    void ParticleSystemData3::BuildNeighborLists(double MaxSearchRadius)
    {
        Timer timer;

        _NeighborLists.resize(NumberOfParticles());
        auto points = Positions();
        for (size_t i = 0; i < NumberOfParticles(); ++i)
        {
            Vector3D origin = points[i];
            _NeighborLists[i].clear();

            _NeighborSearch->ForEachNearbyPoint(origin, MaxSearchRadius,
                            [&](size_t j, const Vector3D&){
                                if (i != j){

                                    _NeighborLists[i].push_back(j);
                                }
                            });
        }

        JET_INFO << "Building Neighbor List took: "
                << timer.DurationInSeconds()
                << " seconds";
    }

    void ParticleSystemData3::Serialize(std::vector<uint8_t>* buffer) const
    {
        flatbuffers::FlatBufferBuilder builder(1024);
        flatbuffers::Offset<fbs::ParticleSystemData3> fbsParticleSystemData;

        SerializeParticleSystemData(&builder, &fbsParticleSystemData);

        builder.Finish(fbsParticleSystemData);

        uint8_t *buf = builder.GetBufferPointer();
        size_t size = builder.GetSize();
        buffer->resize(size);
        memcpy(buffer->data(), buf, size);
    }

    void ParticleSystemData3::Deserialize(const std::vector<uint8_t>& buffer)
    {
        auto fbsParticleSystemData = fbs::GetParticleSystemData3(buffer.data());
        DeserializeParticleSystemData(fbsParticleSystemData);
    }

    void ParticleSystemData3::Set(const ParticleSystemData3& other)
    {
        _Radius = other._Radius;
        _Mass = other._Mass;
        _PositionIdx = other._PositionIdx;
        _VelocityIdx = other._VelocityIdx;
        _ForceIdx = other._ForceIdx;
        _NumberOfParticles = other._NumberOfParticles;

        for (auto& attr : other._ScalarDataList)
            _ScalarDataList.emplace_back(attr);
        
        for (auto& attr : other._VectorDataList)
            _VectorDataList.emplace_back(attr);

        _NeighborSearch = other._NeighborSearch->Clone();
        _NeighborLists = other._NeighborLists;
    }

    ParticleSystemData3& ParticleSystemData3::operator=(const ParticleSystemData3& other)
    {
        Set(other);
        return *this;
    }

    void ParticleSystemData3::SerializeParticleSystemData(flatbuffers::FlatBufferBuilder* builder,
                    flatbuffers::Offset<fbs::ParticleSystemData3>* fbsParticleSystemData) const
    {
        //Copy Data.
        std::vector<flatbuffers::Offset<fbs::ScalarParticleData3>> scalarDataList;
        for (const auto& scalarData: _ScalarDataList)
        {
            auto fbsScalarData = fbs::CreateScalarParticleData3(*builder, builder->CreateVector(scalarData.Data(), scalarData.Size()));
            scalarDataList.push_back(fbsScalarData);
        }

        auto fbsScalarDataList = builder->CreateVector(scalarDataList);

        std::vector<flatbuffers::Offset<fbs::VectorParticleData3>> vectorDataList;
        for (const auto& vectorData: _VectorDataList)
        {
            std::vector<fbs::Vector3D> newVectorData;
            for (const auto& v : vectorData)
            {
                newVectorData.push_back(JetToFbs(v));
            }

            auto fbsVectorData = fbs::CreateVectorParticleData3(
                                *builder, builder->CreateVectorOfStructs(newVectorData.data(), newVectorData.size()));
            
            vectorDataList.push_back(fbsVectorData);
        }
        auto fbsVectorDataList = builder->CreateVector(vectorDataList);

        // Copy Neighbor Search
        auto NeighborSearchType = builder->CreateString(_NeighborSearch->TypeName());
        std::vector<uint8_t> neighborSearchSerialized;
        _NeighborSearch->Serialize(&neighborSearchSerialized);
        auto fbsNeighborSearch = fbs::CreatePointNeighborSearcherSerialized3(*builder,
                                    NeighborSearchType, 
                                    builder->CreateVector(neighborSearchSerialized.data(),
                                    neighborSearchSerialized.size()));

        //Copy Neighbor Lists.
        std::vector<flatbuffers::Offset<fbs::ParticleNeighborList3>> NeighborLists;
        for (const auto& neighbors: _NeighborLists)
        {
            std::vector<uint64_t> Neighbors64(neighbors.begin(), neighbors.end());
            flatbuffers::Offset<fbs::ParticleNeighborList3> fbsNeighborList
                = fbs::CreateParticleNeighborList3(*builder, 
                    builder->CreateVector(Neighbors64.data(), Neighbors64.size()));
            NeighborLists.push_back(fbsNeighborList);
        }

        auto fbsNeighborLists = builder->CreateVector(NeighborLists);

        //Copy the Search object.
        *fbsParticleSystemData = fbs::CreateParticleSystemData3(
                                *builder, _Radius, _Mass, _PositionIdx, _VelocityIdx,
                                _ForceIdx, fbsScalarDataList, fbsVectorDataList,
                                fbsNeighborSearch, fbsNeighborLists);
    }

    void ParticleSystemData3::DeserializeParticleSystemData(const fbs::ParticleSystemData3* fbsParticleSystemData)
    {
        _ScalarDataList.clear();
        _VectorDataList.clear();

        //Copy Scalars
        _Radius = fbsParticleSystemData->radius();
        _Mass = fbsParticleSystemData->mass();
        _PositionIdx = static_cast<size_t>(fbsParticleSystemData->positionIdx());
        _VelocityIdx = static_cast<size_t>(fbsParticleSystemData->velocityIdx());
        _ForceIdx = static_cast<size_t>(fbsParticleSystemData->forceIdx());

        //Copy Data
        auto fbsScalarDataList = fbsParticleSystemData->scalarDataList();
        for (const auto& fbsScalarData : (*fbsScalarDataList))
        {
            auto data = fbsScalarData->data();

            _ScalarDataList.push_back(ScalarData(data->size()));

            auto& newData = *(_ScalarDataList.rbegin());

            for (uint32_t i =0; i < data->size(); ++i)
                newData[i] = data->Get(i);
        }

        auto fbsVectorDataList = fbsParticleSystemData->vectorDataList();
        for(const auto& fbsVectorData : (*fbsVectorDataList))
        {
            auto data = fbsVectorData->data();

            _VectorDataList.push_back(VectorData(data->size()));
            auto& newData = *(_VectorDataList.rbegin());
            for (uint32_t i =0; i < data->size(); ++i)
                newData[i] = FbsToJet(*data->Get(i));
        }

        _NumberOfParticles = _VectorDataList[0].Size();

        //Copy Neighbor Search
        auto fbsNeighborSearch = fbsParticleSystemData->neighborSearcher();
        _NeighborSearch = Factory::BuildPointNeighborSearch3(fbsNeighborSearch->type()->c_str());
        std::vector<uint8_t> neighborSearchSerialized(fbsNeighborSearch->data()->begin(),
                                fbsNeighborSearch->data()->end());

        _NeighborSearch->Deserialize(neighborSearchSerialized);

        //Copy Neighbor List
        auto fbsNeighborLists = fbsParticleSystemData->neighborLists();
        _NeighborLists.resize(fbsNeighborLists->size());
        for (uint32_t i = 0; i < fbsNeighborLists->size(); ++i)
        {
            auto fbsNeighborList = fbsNeighborLists->Get(i);
            _NeighborLists[i].resize(fbsNeighborList->data()->size());
            std::transform(fbsNeighborList->data()->begin(),
            fbsNeighborList->data()->end(),
            _NeighborLists[i].begin(),
            [](uint64_t val)
            {
                return static_cast<size_t>(val);
            });
        }

    }

}