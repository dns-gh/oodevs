#include "CollisionProperty.h"

Geometry2D::Circle CollisionProperty::GetShape() const
{
    return shape_;
}

CollisionFilter CollisionProperty::GetFilter() const
{
    return filter_;
}

void CollisionProperty::SetCollisionShape( const Geometry2D::Circle& circle )
{
    shape_ = circle;
}

void CollisionProperty::SetCollisionFilter( const CollisionFilter& filter )
{
    filter_ = filter;
}
