#pragma once

#include "generated/basic_types_generated.h"
#include <Size/size2.h>
#include <Size/size3.h>
#include <Vector/vector2.h>
#include <Vector/vector3.h>
#include <algorithm>
#include <vector>


namespace jet
{
    inline fbs::Size2 JetToFbs(const Size2& vec)
    {
        return fbs::Size2(vec.x, vec.y);
    }

    inline fbs::Size3 JetToFbs(const Size3& vec)
    {
        return fbs::Size3(vec.x, vec.y, vec.z);
    }

    inline fbs::Vector2D JetToFbs(const Vector2D& vec)
    {
        return fbs::Vector2D(vec.x, vec.y);
    }

    inline fbs::Vector3D JetToFbs(const Vector3D& vec)
    {
        return fbs::Vector3D(vec.x, vec.y, vec.z);
    }

    inline Size2 FbsToJet(const fbs::Size2& vec)
    {
        return Size2({vec.x(), vec.y()});
    }

    inline Size3 FbsToJet(const fbs::Size3& vec)
    {
        return Size3({vec.x(), vec.y(), vec.z()});
    }

    inline Vector2D FbsToJet(const fbs::Vector2D& vec)
    {
        return Vector2D(vec.x(), vec.y());
    }

    inline Vector3D FbsToJet(const fbs::Vector3D& vec)
    {
        return Vector3D(vec.x(), vec.y(). vec.z());
    }


    template<typename GridType,
            typename FbsFactoryFunc,
            typename FbsGridType>
    void SerializeGrid(flatbuffers::FlatBufferBuilder* builder,
                        const std::vector<GridType>& GridList,
                        FbsFactoryFunc func,
                        std::vector<flatbuffers::Offset<FbsGridType>>* fbsGridList)
    {
        for(const auto& grid: GridList)
        {
            auto type = builder->CreateString(grid->typeName());
            
            std::vector<uint8_t> GridSerialized;
            grid->Serialize(&GridSerialized);
            auto fbsGrid = func(*builder, type, 
                                    builder->CreateVector(GridSerialized.data(), GridSerialized.size()));
            fbsGridList->push_back(fbsGrid);
        }
    }


    template <typename FbsGridList, typename GridType, typename FactoryFunc>
    void DeserializeGrid(FbsGridList* fbsGridList, FactoryFunc factoryFunc,
                            std::vector<GridType>* GridList)
    {
        for (const auto& grid : (*fbsGridList))
        {
            auto type = grid->type()->c_str();

            std::vector<uint8_t> GridSerialized(
                grid->data()->begin(),
                grid->data()->end()
            );

            auto NewGrid = factoryFunc(type);
            NewGrid->Deserialize(GridSerialized);

            GridList->push_back(NewGrid);
        }
    }
    
}