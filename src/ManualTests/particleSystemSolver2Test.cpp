#include "manual_tests.h"

#include <ParticleSim/Collision/rigid_body2_collider.h>
#include <Field/VectorField/constant_vector_field2.h>
#include <ParticleSim/particle_system_solver2.h>
#include <Geometry/Plane/plane2.h>
#include <ParticleSim/ParticleEmitter/point_particle_emitter2.h>

using namespace jet;

JET_TESTS(ParticleSystemSolver2);

JET_BEGIN_TEST_F(ParticleSystemSolver2, Update) {
    Plane2Ptr plane = std::make_shared<Plane2>(Vector2D(0, 1), Vector2D());
    RigidBodyCollider2Ptr collider
        = std::make_shared<RigidBodyCollider2>(plane);
    ConstantVectorField2Ptr wind
        = std::make_shared<ConstantVectorField2>(Vector2D(1, 0));

    ParticleSystemSolver2 solver;
    solver.SetCollider(collider);
    solver.SetWind(wind);

    ParticleSystemData2Ptr particles = solver.ParticleSystemData();
    PointParticleEmitter2Ptr emitter
        = std::make_shared<PointParticleEmitter2>(
            Vector2D(0, 3),
            Vector2D(0, 1), 5.0, 45.0);
    emitter->SetMaxParticleRate(100);
    solver.SetEmitter(emitter);

    SaveParticleDataXy(particles, 0);

    Frame frame(1, 1.0 / 60.0);
    for ( ; frame.Index < 360; frame.Advance()) {
        solver.Update(frame);

        SaveParticleDataXy(particles, frame.Index);
    }
}
JET_END_TEST_F