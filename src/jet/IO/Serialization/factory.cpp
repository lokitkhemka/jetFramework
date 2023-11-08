#include <jet.h>
#include "factory.h"

#include <NeighborhoodSearch/point2_hash_grid_search.h>
#include <NeighborhoodSearch/point2_parallel_hash_grid_search.h>

#include <string>
#include <unordered_map>

namespace jet
{
    namespace
    {
        std::unordered_map<std::string, PointNeighborSearchBuilder2Ptr> sPointNeighborSerach2Builders;
    }

    #define REGISTER_BUILDER(map, ClassName) \
            map.emplace(#ClassName, std::make_shared<ClassName::Builder>());

    #define REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(ClassName) \
            REGISTER_BUILDER(sPointNeighborSerach2Builders, ClassName)

    
    class Registry
    {
    public:
        Registry()
        {
            REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(PointHashGridSearch2)
            REGISTER_POINT_NEIGHBOR_SEARCH2_BUILDER(PointParallelHashGridSearch2)
        }
    };

    static Registry sRegistry;

    PointNeighborSearch2Ptr Factory::BuildPointNeighborSearch2(const std::string& name)
    {
        auto result = sPointNeighborSerach2Builders.find(name);
        if (result != sPointNeighborSerach2Builders.end())
        {
            auto builder = result->second;
            return builder->BuildPointNeighborSearch();
        }
        else
        {
            return nullptr;
        }
    }
}