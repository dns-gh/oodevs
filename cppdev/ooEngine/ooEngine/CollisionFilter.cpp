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

// returns if a collision is possible
bool CollisionFilter::Filters( const CollisionFilter& filter )
{
    // group index overrides categories and filters
    if( groupIndex_ == 0 || filter.GetGroupIndex() == 0 )
        return false;

    if( groupIndex_ == 1 || filter.GetGroupIndex() == 1 )
        return true;

    if( ( groupIndex_ & filter.GetGroupIndex() ) != 0 )
        return true;

    // category and mask filters
    bool collision = ( ( filter.GetCategory() & collisionMask_ ) != 0 ) &&
                        ( ( category_ & filter.GetCollisionMask() ) != 0 );
    if( !collision )
        return false;
    return true;
}