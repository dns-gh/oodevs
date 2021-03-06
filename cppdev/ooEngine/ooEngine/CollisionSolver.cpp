#include "CollisionSolver.h"

CollisionSolver::CollisionSolver( LogTools& logger )
    : logger_( logger )
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Collision solver: creation (id:" << GetID() << ")" );
}

bool CollisionSolver::CollisionBetween( const Geometry2D::Circle& c1, const Geometry2D::Circle& c2 ) const
{
    // no collision if the collision circle is a point
    if( c1.radius_ == 0 || c2.radius_ == 0 )
        return false;
    if( Geometry2D::Distance( c1.center_, c2.center_ ) <= c1.radius_ + c2.radius_ )
        return true;
    return false;
}