#include "CollisionProperty.h"

Geometry2D::Circle CollisionProperty::GetCircleCollisionShape() const
{
    return circleShape_;
}

CollisionFilter CollisionProperty::GetFilter() const
{
    return filter_;
}

void CollisionProperty::SetCircleCollisionShape( const Geometry2D::Circle& circle )
{
    circleShape_ = circle;
}

void CollisionProperty::SetCollisionFilter( const CollisionFilter& filter )
{
    filter_ = filter;
}
