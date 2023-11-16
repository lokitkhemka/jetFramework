#include <jet.h>
#include "bcc_lattice_point_generator.h"

namespace jet
{
    BCCLatticePointGenerator::BCCLatticePointGenerator() 
    {}

    BCCLatticePointGenerator::~BCCLatticePointGenerator() 
    {}

    void BCCLatticePointGenerator::Generate(
        const BoundingBox3D& boundingBox,
        double spacing,
        Array1<Vector3D>* points) const 
    {
        ForEachPoint(
            boundingBox,
            spacing,
            [&points](const Vector3D& point) {
                points->Append(point);
                return true;
            });
    }

    void BCCLatticePointGenerator::ForEachPoint(
        const BoundingBox3D& boundingBox, double spacing,
        const std::function<bool(const Vector3D&)>& callback) const
    {
        double HalfSpacing = spacing/2.0;
        double BoxWidth = boundingBox.Width();
        double BoxHeight = boundingBox.Height();
        double BoxDepth = boundingBox.Depth();

        Vector3D position;
        bool HasOffset = false;
        bool ShouldQuit = false;
        for (int k = 0; k * HalfSpacing <= BoxDepth && !ShouldQuit; ++k)
        {
            position.z = k * HalfSpacing + boundingBox.LowerCorner.z;
            double offset = (HasOffset) ? HalfSpacing : 0.0;

            for (int j = 0; j * spacing + offset <= BoxHeight && !ShouldQuit; ++j)
            {
                position.y = j * spacing + offset + boundingBox.LowerCorner.y;

                for (int i = 0; i * spacing + offset <= BoxWidth; ++i)
                {
                    position.x = i * spacing + offset + boundingBox.LowerCorner.x;
                    if (!callback(position))
                    {
                        ShouldQuit = true;
                        break;
                    }
                }
            }
            HasOffset = !HasOffset;
        }
    }
}