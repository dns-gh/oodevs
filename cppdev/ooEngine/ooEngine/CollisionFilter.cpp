#include "CollisionFilter.h"

CollisionFilter::CollisionFilter()
{
    category_ = CollisionCategory::Default;
    collisionMask_ = CollisionCategory::Default;
    groupIndex_ = 0;
}

CollisionFilter::CollisionFilter( unsigned int category, unsigned int mask, unsigned int groupeIndex )
    : category_( category )
    , collisionMask_( mask )
    , groupIndex_( groupeIndex )
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

unsigned int CollisionFilter::GetGroupIndex() const
{
    return groupIndex_;
}