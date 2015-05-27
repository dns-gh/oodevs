#include "CollisionFilter.h"

CollisionFilter::CollisionFilter()
{
    category_ = CollisionGroup::Default;
    collisionMask_ = CollisionGroup::Default;
}

CollisionFilter::CollisionFilter( unsigned int category, unsigned int mask )
    : category_( category )
    , collisionMask_( mask )
{
    // NOTHING
}

void CollisionFilter::UpdateCategory( unsigned int category )
{
    category_ = category;
}

void CollisionFilter::UpdateCollisionMask( unsigned int mask )
{
    collisionMask_ = mask;
}

//void UnionCategory( unsigned int );
//void UnionCollisionMask( unsigned int );

unsigned int CollisionFilter::GetCategory() const
{
    return category_;
}

unsigned int CollisionFilter::GetCollisionMask() const
{
    return collisionMask_;
}