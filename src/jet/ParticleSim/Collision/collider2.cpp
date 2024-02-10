// #include <jet.h>
// #include "collider2.h"
// #include <algorithm>

// namespace jet
// {
//     Collider2::Collider2()
//     {}

//     Collider2::~Collider2()
//     {}

//     void Collider2::ResolveCollision(double radius, double restitutionCoefficient,
//                     Vector2D* newPosition, Vector2D* newVelocity)
//     {
//         ColliderQueryResult colliderPoint;

//         GetClosestPoint(_Surface, *newPosition, &colliderPoint);

//         // Check if the new position is penetrating the surface.
//         if (IsPenetrating(colliderPoint, *newPosition, radius))
//         {
//             // Target point is the closest non-penetrating position from the new position.
            
//         }
//     }
// }