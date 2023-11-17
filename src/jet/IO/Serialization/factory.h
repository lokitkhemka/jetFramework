#pragma once

#include <NeighborhoodSearch/point2_neighbor_search.h>
#include <NeighborhoodSearch/point3_neighbor_search.h>
#include <string>

namespace jet
{
    class Factory
    {
    public:
        static PointNeighborSearch2Ptr BuildPointNeighborSearch2(const std::string& name);

        static PointNeighborSearch3Ptr BuildPointNeighborSearch3(const std::string& name);
    };
}