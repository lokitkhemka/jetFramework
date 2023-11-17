#include <jet.h>
#include "factory.h"

#include <NeighborhoodSearch/point2_hash_grid_search.h>
#include <NeighborhoodSearch/point2_parallel_hash_grid_search.h>
#include <NeighborhoodSearch/point3_hash_grid_search.h>
#include <NeighborhoodSearch/point3_parallel_hash_grid_search.h>

#include <string>
#include <unordered_map>

namespace jet
{
    namespace
    {
        std::unordered_map<std::string, PointNeighborSearchBuilder2Ptr> sPointNeighborSearch2Builders;
        std::unordered_map<std::string, PointNeighborSearchBuilder3Ptr> sPointNeighborSearch3Builders;
    }

    #define REGISTER_BUILDER(map, ClassName) \
            map.emplace(#ClassName, std::make_shared<ClassName::Builder>());

    #define REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(ClassName) \
            REGISTER_BUILDER(sPointNeighborSearch2Builders, ClassName)
    
    #define REGISTER_POINT_NEIGHBOR_SEARCH3_BUILDER(ClassName) \
            REGISTER_BUILDER(sPointNeighborSearch3Builders, ClassName)

    
    class Registry
    {
    public:
        Registry()
        {
            REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(PointHashGridSearch2)
            REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(PointParallelHashGridSearch2)
            
            REGISTER_POINT_NEIGHBOR_SEARCH3_BUILDER(PointHashGridSearch3)
            REGISTER_POINT_NEIGHBOR_SEARCH3_BUILDER(PointParallelHashGridSearch3)
        }
    };

    static Registry sRegistry;

    PointNeighborSearch2Ptr Factory::BuildPointNeighborSearch2(const std::string& name)
    {
        auto result = sPointNeighborSearch2Builders.find(name);
        if (result != sPointNeighborSearch2Builders.end())
        {
            auto builder = result->second;
            return builder->BuildPointNeighborSearch();
        }
        else
        {
            return nullptr;
        }
    }

    PointNeighborSearch3Ptr Factory::BuildPointNeighborSearch3(
    const std::string& name) {
    auto result = sPointNeighborSearch3Builders.find(name);
    if (result != sPointNeighborSearch3Builders.end()) {
        auto builder = result->second;
        return builder->BuildPointNeighborSearch();
    } else {
        return nullptr;
    }
}
}