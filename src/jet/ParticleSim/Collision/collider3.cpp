#include <jet.h>

#include "collider3.h"
#include <algorithm>

namespace jet
{
    Collider3::Collider3()
    {}

    Collider3::~Collider3()
    {}

    void Collider3::ResolveCollision(double radius, double restitutionCoeff,
                                    Vector3D* newPos, Vector3D* newVelocity)
    {
        ColliderQueryResult ColliderPoint;

        GetClosestPoint(_Surface, *newPos, &ColliderPoint);

        if (IsPenetrating(ColliderPoint, *newPos, radius))
        {
            // Target point is the closet non-penetrating position from the new position.
            Vector3D targetNormal = ColliderPoint.Normal;
            Vector3D targetPoint = ColliderPoint.Point + radius * targetNormal;
            Vector3D colliderVelAtTargetPoint = ColliderPoint.Velocity;

            // Get new candidate relative velocity from the target point.
            Vector3D relativeVel = *newVelocity - colliderVelAtTargetPoint;
            double normalDotRelativeVel = targetNormal.Dot(relativeVel);
            Vector3D relativeVelN = normalDotRelativeVel * targetNormal;
            Vector3D relativeVelT = relativeVel - relativeVelN;

            // Check if the velocity is facing opposite direction of the surface normal
            if (normalDotRelativeVel < 0.0)
            {
                // Apply restitution coefficientto the surface normal component of the velocity
                Vector3D deltaRelativeVelN = (-restitutionCoeff - 1.0) * relativeVelN;
                relativeVelN *= -restitutionCoeff;

                // Apply friction to the tangential component of the velocity.
                if (relativeVelT.LengthSquared() > 0.0)
                {
                    double frictionScale = std::max(1.0 - _FrictionCoefficient * 
                                            deltaRelativeVelN.Length()/relativeVelN.Length(), 0.0);
                    relativeVelT *= frictionScale;
                }

                // Reassemble the components
                *newVelocity = relativeVelN + relativeVelT + colliderVelAtTargetPoint;
            }

            // Geometric fix
            *newPos = targetPoint;

        }
    }

    double Collider3::FrictionCoefficient() const
    {
        return _FrictionCoefficient;
    }

    void Collider3::SetFrictionCoefficient(double newFrictionCoeff)
    {
        _FrictionCoefficient = std::max(newFrictionCoeff, 0.0);
    }

    const Surface3Ptr& Collider3::Surface() const
    {
        return _Surface;
    }

    void Collider3::SetSurface(const Surface3Ptr& newSurface)
    {
        _Surface = newSurface;
    }

    void Collider3::GetClosestPoint(const Surface3Ptr& surface, const Vector3D& queryPoint,
                ColliderQueryResult* result) const
    {
        result->Distance = surface->ClosestDistance(queryPoint);
        result->Point = surface->ClosestPoint(queryPoint);
        result->Normal = surface->ClosestNormal(queryPoint);
        result->Velocity = VelocityAt(queryPoint);
    }

    bool Collider3::IsPenetrating(const ColliderQueryResult& colliderPoint,
                                    const Vector3D& position, double radius)
    {
        return (position - colliderPoint.Point).Dot(colliderPoint.Normal) < 0.0 ||
                colliderPoint.Distance < radius;
    }

    void Collider3::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
    {
        if (_OnUpdateCallback)
        {
            _OnUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
        }
    }

    void Collider3::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
    {
        _OnUpdateCallback = callback;
    }
}