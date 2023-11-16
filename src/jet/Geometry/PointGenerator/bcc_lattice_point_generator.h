#pragma once

#include <Arrays/array1.h>
#include <Geometry/BoundingBox/bounding_box3.h>
#include <functional>
#include <memory>

namespace jet
{
    //! \brief Body-centered lattice points generation class.
    class BCCLatticePointGenerator
    {
    public:
        BCCLatticePointGenerator();

        ~BCCLatticePointGenerator();
        //! Generates point to output array \p points inside given \p BoundingBox
        //! with target point \p spacing.
        void Generate(const BoundingBox3D& boundingBox, double spacing,
                        Array1<Vector3D>* points) const;
        
        //! \brief Invokes \p callback function for each BCC-lattice points inside
        //! \p boundingBox.
        //!
        //! This function iterates every BCC-lattice points inside \p boundingBox
        //! where \p spacing is the size of the unit cell of BCC structure.
        void ForEachPoint(const BoundingBox3D& boundingBox, double spacing,
                            const std::function<bool(const Vector3D&)>& callback) const;
    };

    typedef std::shared_ptr<BCCLatticePointGenerator> BCCLatticePointGeneratorPtr;
}