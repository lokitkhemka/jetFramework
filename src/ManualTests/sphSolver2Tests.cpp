#include "manual_tests.h"

#include <Geometry/Box/box2.h>
#include <Geometry/Plane/plane2.h>
#include <ParticleSim/Collision/rigid_body2_collider.h>
#include <ParticleSim/SPH/sph_solver2.h>
#include <Geometry/Surface/surface_to_implicit2.h>
#include <Geometry/Sphere/sphere2.h>
#include <Geometry/PointGenerator/volume_particle_emitter2.h>
#include <Geometry/ImplicitSurface/implicit_surface2_set.h>


using namespace jet;

JET_TESTS(SPHSolver2)

JET_BEGIN_TEST_F(SPHSolver2, WaterDrop) {
    const double targetSpacing = 0.02;

    BoundingBox2D domain(Vector2D(), Vector2D(1, 2));

    // Initialize solvers
    SPHSolver2 solver;
    solver.SetPseudoViscosityCoefficient(0.0);

    SPHSystemData2Ptr particles = solver.SPHSystemData();
    particles->SetTargetDensity(1000.0);
    particles->SetTargetSpacing(targetSpacing);

    // Initialize source
    ImplicitSurfaceSet2Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet2>();
    surfaceSet->AddExplicitSurface(
        std::make_shared<Plane2>(
            Vector2D(0, 1), Vector2D(0, 0.25 * domain.Height())));
    surfaceSet->AddExplicitSurface(
        std::make_shared<Sphere2>(
            domain.MidPoint(), 0.15 * domain.Width()));

    BoundingBox2D sourceBound(domain);
    sourceBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter2>(
        surfaceSet,
        sourceBound,
        targetSpacing,
        Vector2D());
    solver.SetEmitter(emitter);

    // Initialize boundary
    Box2Ptr box = std::make_shared<Box2>(domain);
    box->IsNormalFlipped = true;
    RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);

    // Setup solver
    solver.SetCollider(collider);

    SaveParticleDataXy(particles, 0);

    Frame frame(1, 1.0 / 60.0);
    for ( ; frame.Index < 120; frame.Advance()) {
        solver.Update(frame);

        SaveParticleDataXy(particles, frame.Index);
    }
}
JET_END_TEST_F