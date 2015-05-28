#ifndef _COLLISION_PROPERTY_H_
#define _COLLISION_PROPERTY_H_

#include "CollisionFilter.h"
#include "Geometry2D.h"

class CollisionProperty
{
public:
    Geometry2D::Circle GetCircleCollisionShape() const;
    CollisionFilter GetFilter() const;

    void SetCircleCollisionShape( const Geometry2D::Circle& circle );
    void SetCollisionFilter( const CollisionFilter& filter );

private:
    CollisionFilter filter_;
    Geometry2D::Circle circleShape_;
};

#endif // CollisionProperty.h