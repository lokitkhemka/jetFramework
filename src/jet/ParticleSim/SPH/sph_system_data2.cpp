#include <jet.h>

#include <IO/Serialization/fbs_helpers.h>
#include <IO/Serialization/generated/sph_system_data2_generated.h>

#include <parallel.h>
#include <ParticleSim/SPH/sph_kernels2.h>
#include "sph_system_data2.h"

#include <Geometry/PointGenerator/triangle_point_generator.h>

#include <algorithm>
#include <vector>

namespace jet
{
    SPHSystemData2::SPHSystemData2()
            :SPHSystemData2(0)
    {}

    SPHSystemData2::SPHSystemData2(size_t NumberOfParticles)
        : ParticleSystemData2(NumberOfParticles)
    {
        _DensityIdx = AddScalarData();
        _PressureIdx = AddScalarData();

        SetTargetSpacing(_TargetSpacing);
    }

    SPHSystemData2::SPHSystemData2(const SPHSystemData2& other)
    {
        Set(other);
    }

    SPHSystemData2::~SPHSystemData2()
    {}

    void SPHSystemData2::SetRadius(double newRadius)
    {
        // Interpreted as setting target spacing
        SetTargetSpacing(newRadius);
    }

    void SPHSystemData2::SetMass(double newMass)
    {
        double incRatio = newMass / Mass();
        _TargetDensity *= incRatio;
        ParticleSystemData2::SetMass(newMass);
    }

    ConstArrayAccessor1<double> SPHSystemData2::Densities() const
    {
        return ScalarDataAt(_DensityIdx);
    }

    ArrayAccessor1<double> SPHSystemData2::Densities()
    {
        return ScalarDataAt(_DensityIdx);
    }

    ConstArrayAccessor1<double> SPHSystemData2::Pressures() const
    {
        return ScalarDataAt(_PressureIdx);
    }

    ArrayAccessor1<double> SPHSystemData2::Pressures()
    {
        return ScalarDataAt(_PressureIdx);
    }

    void SPHSystemData2::UpdateDensities()
    {
        auto p = Positions();
        auto d = Densities();
        const double m = Mass();

        ParallelFor(kZeroSize, NumberOfParticles(),
                        [&](size_t i)
                        {
                            double sum = SumOfKernelsNearby(p[i]);
                            d[i] = m * sum;
        });
    }

    void SPHSystemData2::SetTargetDensity(double targetDensity)
    {
        _TargetDensity = targetDensity;
        ComputeMass();
    }

    double SPHSystemData2::TargetDensity() const
    {
        return _TargetDensity;
    }

    void SPHSystemData2::SetTargetSpacing(double spacing)
    {
        ParticleSystemData2::SetRadius(spacing);

        _TargetSpacing = spacing;
        _KernelRadius = _RelativeRadius * _TargetSpacing;

        ComputeMass();
    }

    double SPHSystemData2::TargetSpacing() const
    {
        return _TargetSpacing;
    }

    void SPHSystemData2::SetRelativeKernelRadius(double relRadius)
    {
        _RelativeRadius = relRadius;
        _KernelRadius = _RelativeRadius * _TargetSpacing;

        ComputeMass();
    }

    double SPHSystemData2::RelativeKernelRadius() const
    {
        return _RelativeRadius;
    }

    double SPHSystemData2::KernelRadius() const
    {
        return _KernelRadius;
    }

    double SPHSystemData2::SumOfKernelsNearby(const Vector2D& origin) const
    {
        double sum = 0.0;
        SPHStdKernel2 kernel(_KernelRadius);
        NeighborSearch()->ForEachNearbyPoint(origin, _KernelRadius,
                        [&](size_t, const Vector2D& neighborPos)
                        {
                            double dist = origin.DistanceTo(neighborPos);
                            sum += kernel(dist);
                        });
        return sum;
    }

    double SPHSystemData2::Interpolate(const Vector2D& origin, const ConstArrayAccessor1<double>& values) const
    {
        double sum = 0.0;
        auto d = Densities();
        SPHStdKernel2 kernel(_KernelRadius);
        const double m = Mass();

        NeighborSearch()->ForEachNearbyPoint(origin, _KernelRadius,
                                [&](size_t i, const Vector2D& neighborPos)
                                {
                                    double dist = origin.DistanceTo(neighborPos);
                                    double weight = m / d[i] * kernel(dist);
                                    sum += weight * values[i];
                                });
        return sum;
    }

    Vector2D SPHSystemData2::Interpolate(const Vector2D& origin, const ConstArrayAccessor1<Vector2D>& values) const
    {
        Vector2D sum;
        auto d = Densities();
        SPHStdKernel2 kernel(_KernelRadius);
        const double m = Mass();

        NeighborSearch()->ForEachNearbyPoint(origin, _KernelRadius,
                        [&](size_t i, const Vector2D& neighborPos)
                        {
                            double dist = origin.DistanceTo(neighborPos);
                            double weight = m / d[i] * kernel(dist);
                            sum += weight * values[i];
                        });
        return sum;
    }

    Vector2D SPHSystemData2::GradientAt(size_t i, const ConstArrayAccessor1<double>& values) const
    {
        Vector2D sum;
        auto p = Positions();
        auto d = Densities();
        const auto& neighbors = NeighborLists()[i];

        Vector2D origin = p[i];
        SPHSpikyKernel2 kernel(_KernelRadius);
        const double m = Mass();

        for (size_t j : neighbors)
        {
            Vector2D neighborPos = p[j];
            double dist = origin.DistanceTo(neighborPos);
            if (dist > 0.0)
            {
                Vector2D dir = (neighborPos - origin) / dist;
                sum += d[i] * m
                    * (values[i] / Square(d[i]) + values[j] / Square(d[j]))
                    * kernel.Gradient(dist,dir);
            }
        }
        return sum;
    }

    double SPHSystemData2::LaplacianAt(size_t i, const ConstArrayAccessor1<double>& values) const
    {
        double sum = 0.0;
        auto p = Positions();
        auto d = Densities();
        const auto& neighbors = NeighborLists()[i];
        Vector2D origin = p[i];
        SPHSpikyKernel2 kernel(_KernelRadius);
        const double m = Mass();

        for (size_t j : neighbors)
        {
            Vector2D neighborPos = p[j];
            double dist = origin.DistanceTo(neighborPos);
            sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
        }

        return sum;
    }

    Vector2D SPHSystemData2::LaplacianAt(size_t i, const ConstArrayAccessor1<Vector2D>& values) const
    {
        Vector2D sum;
        auto p = Positions();
        auto d = Densities();
        const auto& neighbors = NeighborLists()[i];
        Vector2D origin = p[i];
        SPHSpikyKernel2 kernel(_KernelRadius);
        const double m = Mass();
        for (size_t j : neighbors)
        {
            Vector2D neighborPos = p[j];
            double dist = origin.DistanceTo(neighborPos);
            sum += m * (values[j] - values[i]) / d[j] * kernel.SecondDerivative(dist);
        }

        return sum;
    }

    void SPHSystemData2::BuildNeighborSearch()
    {
        ParticleSystemData2::BuildNeighborSearch(_KernelRadius);
    }

    void SPHSystemData2::BuildNeighborLists()
    {
        ParticleSystemData2::BuildNeighborLists(_KernelRadius);
    }

    void SPHSystemData2::ComputeMass()
    {
        Array1<Vector2D> points;
        TrianglePointGenerator pointsGenerator;
        BoundingBox2D sampleBound(Vector2D(-1.5 * _KernelRadius, -1.5 * _KernelRadius), 
                                Vector2D(1.5 * _KernelRadius, 1.5 * _KernelRadius));
        
        pointsGenerator.Generate(sampleBound, _TargetSpacing, &points);

        double maxNumberDensity = 0.0;

        SPHStdKernel2 kernel(_KernelRadius);

        for (size_t i = 0; i < points.Size(); ++i)
        {
            const Vector2D& point = points[i];
            double sum = 0.0;

            for (size_t j = 0; j < points.Size(); ++j)
            {
                const Vector2D& neighborPoint = points[j];
                sum += kernel(neighborPoint.DistanceTo(point));
            }

            maxNumberDensity = std::max(maxNumberDensity, sum);
        }

        JET_ASSERT(maxNumberDensity > 0);
        double newMass = _TargetDensity / maxNumberDensity;
        ParticleSystemData2::SetMass(newMass);
    }

    void SPHSystemData2::Serialize(std::vector<uint8_t>* buffer) const {
    flatbuffers::FlatBufferBuilder builder(1024);
    flatbuffers::Offset<fbs::ParticleSystemData2> fbsParticleSystemData;

    SerializeParticleSystemData(&builder, &fbsParticleSystemData);

    auto fbsSphSystemData = fbs::CreateSphSystemData2(
        builder,
        fbsParticleSystemData,
        _TargetDensity,
        _TargetSpacing,
        _RelativeRadius,
        _KernelRadius,
        _PressureIdx,
        _DensityIdx);

    builder.Finish(fbsSphSystemData);

    uint8_t *buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void SPHSystemData2::Deserialize(const std::vector<uint8_t>& buffer) {
    auto fbsSphSystemData = fbs::GetSphSystemData2(buffer.data());

    auto base = fbsSphSystemData->base();
    DeserializeParticleSystemData(base);

    // SPH specific
    _TargetDensity = fbsSphSystemData->targetDensity();
    _TargetSpacing = fbsSphSystemData->targetSpacing();
    _RelativeRadius
        = fbsSphSystemData->kernelRadiusOverTargetSpacing();
    _KernelRadius = fbsSphSystemData->kernelRadius();
    _PressureIdx = static_cast<size_t>(fbsSphSystemData->pressureIdx());
    _DensityIdx = static_cast<size_t>(fbsSphSystemData->densityIdx());
}

void SPHSystemData2::Set(const SPHSystemData2& other) {
    ParticleSystemData2::Set(other);

    _TargetDensity = other._TargetDensity;
    _TargetSpacing = other._TargetSpacing;
    _RelativeRadius = other._RelativeRadius;
    _KernelRadius = other._KernelRadius;
    _DensityIdx = other._DensityIdx;
    _PressureIdx = other._PressureIdx;
}

SPHSystemData2& SPHSystemData2::operator=(const SPHSystemData2& other) {
    Set(other);
    return *this;
}

}