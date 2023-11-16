#pragma once

#include "point3_generator.h"

namespace jet
{
    //! \brief Body-centered lattice points generation class.
    class BCCLatticePointGenerator final : public PointGenerator3
    {
        //! \brief Invokes \p callback function for each BCC-lattice points inside
        //! \p boundingBox.
        //!
        //! This function iterates every BCC-lattice points inside \p boundingBox
        //! where \p spacing is the size of the unit cell of BCC structure.
        void ForEachPoint(const BoundingBox3D& boundingBox, double spacing,
                            const std::function<bool(const Vector3D&)>& callback) const override;
    };

    typedef std::shared_ptr<BCCLatticePointGenerator> BCCLatticePointGeneratorPtr;
}