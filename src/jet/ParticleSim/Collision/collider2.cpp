#include <jet.h>
#include "collider2.h"
#include <algorithm>

namespace jet
{
    Collider2::Collider2()
    {}

    Collider2::~Collider2()
    {}

    void Collider2::ResolveCollision(double radius, double restitutionCoefficient,
                    Vector2D* newPosition, Vector2D* newVelocity)
    {
        ColliderQueryResult colliderPoint;

        GetClosestPoint(_Surface, *newPosition, &colliderPoint);

        // Check if the new position is penetrating the surface.
        if (IsPenetrating(colliderPoint, *newPosition, radius))
        {
            // Target point is the closest non-penetrating position from the new position.
            Vector2D TargetNormal = colliderPoint.Normal;
            Vector2D TargetPoint = colliderPoint.Point + radius * TargetNormal;
            Vector2D colliderVelAtTargetPoint = colliderPoint.Velocity;

            // Get New Candidate relative velocity from the target point
            Vector2D RelativeVelocity = *newVelocity - colliderVelAtTargetPoint;
            double normalDotRelativeVel = TargetNormal.Dot(RelativeVelocity);
            Vector2D RelativeVelN = normalDotRelativeVel * TargetNormal;
            Vector2D RelativeVelT = RelativeVelocity - RelativeVelN;

            // Checking if the velocity is facing opposite direction fo the surface normal.
            if (normalDotRelativeVel < 0.0)
            {
                // Applying the restitution coefficient to the surface normal component of hte velocity
                Vector2D deltaRelativeVelN = (-restitutionCoefficient - 1.0) * RelativeVelN;
                RelativeVelN *= -restitutionCoefficient;

                //Applying friction to the tangential component of the velocity.
                if (RelativeVelT.LengthSquared() > 0.0)
                {
                    double FrictionScale = std::max(1.0 - 
                            _FrictionCoefficient * deltaRelativeVelN.Length()/RelativeVelT.Length(),0.0);
                    RelativeVelT *= FrictionScale;
                }

                // Reassembling the component
                *newVelocity = RelativeVelN + RelativeVelT + colliderVelAtTargetPoint;
            }

            // Geometric Fix
            *newPosition = TargetPoint;
            
        }
    }

    double Collider2::FrictionCoefficient() const
    {
        return _FrictionCoefficient;
    }

    void Collider2:: SetFrictionCoefficient(double NewFrictionCoefficient)
    {
        _FrictionCoefficient = std::max(NewFrictionCoefficient, 0.0);
    }

    const Surface2Ptr& Collider2::Surface() const
    {
        return _Surface;
    }

    void Collider2::SetSurface(const Surface2Ptr& NewSurface)
    {
        _Surface = NewSurface;
    }

    void Collider2::GetClosestPoint(const Surface2Ptr& surface, const Vector2D& queryPoint, ColliderQueryResult* result) const
    {
        result->Distance = surface->ClosestDistance(queryPoint);
        result->Point = surface->ClosestPoint(queryPoint);
        result->Normal = surface->ClosestNormal(queryPoint);
        result->Velocity = VelocityAt(queryPoint);
    }

    bool Collider2::IsPenetrating(const ColliderQueryResult& colliderPoint, const Vector2D& position, double radius)
    {
        // If the new candidate position of the aprticle is on the other side of surface or the new distance to the
        // surface is less than the particle's radius.
        return (position - colliderPoint.Point).Dot(colliderPoint.Normal) < 0.0 || colliderPoint.Distance < radius;
    }

    void Collider2::Update(double CurrentTimeInSeconds, double TimeIntervalInSeconds)
    {
        if (_OnUpdateCallback)
        {
            _OnUpdateCallback(this, CurrentTimeInSeconds, TimeIntervalInSeconds);
        }
    }

    void Collider2::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
    {
        _OnUpdateCallback = callback;
    }
}