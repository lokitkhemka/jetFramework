#include <Geometry/Sphere/sphere2.h>
#include <Geometry/Surface/surface_to_implicit2.h>
#include <Geometry/PointGenerator/volume_particle_emitter2.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(VolumeParticleEmitter2, Constructors) {
    auto sphere = std::make_shared<SurfaceToImplicit2>(
        std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0));

    VolumeParticleEmitter2 emitter(
        sphere,
        BoundingBox2D({0.0, 0.0}, {3.0, 3.0}),
        0.1,
        {-1.0, 0.5},
        30,
        0.01,
        false,
        true);

    EXPECT_EQ(0.01, emitter.Jitter());
    EXPECT_FALSE(emitter.IsOneShot());
    EXPECT_TRUE(emitter.AllowOverlapping());
    EXPECT_EQ(30u, emitter.MaxNumberOfParticles());
    EXPECT_EQ(0.1, emitter.Spacing());
    EXPECT_EQ(-1.0, emitter.InitialVelocity().x);
    EXPECT_EQ(0.5, emitter.InitialVelocity().y);
}

TEST(VolumeParticleEmitter2, Emit) {
    auto sphere = std::make_shared<SurfaceToImplicit2>(
        std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0));

    BoundingBox2D box({0.0, 0.0}, {3.0, 3.0});

    VolumeParticleEmitter2 emitter(
        sphere,
        box,
        0.3,
        {-1.0, 0.5},
        30,
        0.0,
        false,
        false);

    auto particles = std::make_shared<ParticleSystemData2>();
    emitter.SetTarget(particles);

    Frame frame(1, 1.0);
    emitter.Update(frame.TimeInSeconds(), frame.TimeIntervalInSeconds);

    auto pos = particles->Positions();
    auto vel = particles->Velocities();

    EXPECT_EQ(30u, particles->NumberOfParticles());
    for (size_t i = 0; i < particles->NumberOfParticles(); ++i) {
        EXPECT_GE(3.0, (pos[i] - Vector2D(1.0, 2.0)).Length());
        EXPECT_TRUE(box.Contains(pos[i]));

        EXPECT_EQ(-1.0, vel[i].x);
        EXPECT_EQ(0.5, vel[i].y);
    }

    ++frame;
    emitter.SetMaxNumberOfParticles(60);
    emitter.Update(frame.TimeInSeconds(), frame.TimeIntervalInSeconds);

    EXPECT_EQ(51u, particles->NumberOfParticles());

    pos = particles->Positions();
    for (size_t i = 0; i < particles->NumberOfParticles(); ++i) {
        pos[i] += Vector2D(2.0, 1.5);
    }

    ++frame;
    emitter.Update(frame.TimeInSeconds(), frame.TimeIntervalInSeconds);
    EXPECT_LT(51u, particles->NumberOfParticles());
}

TEST(VolumeParticleEmitter2, Builder) {
    auto sphere = std::make_shared<Sphere2>(Vector2D(1.0, 2.0), 3.0);

    VolumeParticleEmitter2 emitter = VolumeParticleEmitter2::builder()
        .WithSurface(sphere)
        .WithMaxRegion(BoundingBox2D({0.0, 0.0}, {3.0, 3.0}))
        .WithSpacing(0.1)
        .WithInitialVelocity({-1.0, 0.5})
        .WithMaxNumberOfParticles(30)
        .WithJitter(0.01)
        .WithIsOneShot(false)
        .WithAllowOverlapping(true)
        .Build();

    EXPECT_EQ(0.01, emitter.Jitter());
    EXPECT_FALSE(emitter.IsOneShot());
    EXPECT_TRUE(emitter.AllowOverlapping());
    EXPECT_EQ(30u, emitter.MaxNumberOfParticles());
    EXPECT_EQ(0.1, emitter.Spacing());
    EXPECT_EQ(-1.0, emitter.InitialVelocity().x);
    EXPECT_EQ(0.5, emitter.InitialVelocity().y);
}