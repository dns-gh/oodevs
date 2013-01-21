// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PathPointAdapter.h"
#include "Hook.h"
#include "Decision/DEC_PathPoint.h"
#include <boost/make_shared.hpp>

using namespace sword;

DECLARE_HOOK( GetPathDestPoint, size_t, ( size_t point ) )
DECLARE_HOOK( GetPathDIAType, const char*, ( size_t point ) )
DECLARE_HOOK( GetPathLimaPoint, unsigned int, ( size_t point ) )
DECLARE_HOOK( GetPathPos, const MT_Vector2D*, ( size_t point ) )
DECLARE_HOOK( GetPathTypeLimaPoint, int, ( size_t point ) )
DECLARE_HOOK( GetPathTypePoint, int, ( size_t point ) )
DECLARE_HOOK( RemovePathPoint, void, ( unsigned int entity, size_t point ) )

//-----------------------------------------------------------------------------
// Name: PathPointAdapter constructor
// Created: MCO 2012-02-03
//-----------------------------------------------------------------------------
PathPointAdapter::PathPointAdapter( unsigned int entity, std::size_t point )
    : entity_( entity )
    , point_ ( point )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PathPointAdapter destructor
// Created: MCO 2012-02-03
//-----------------------------------------------------------------------------
PathPointAdapter::~PathPointAdapter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::RemoveFromPointsCategory
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
void PathPointAdapter::RemoveFromPointsCategory() const
{
    GET_HOOK( RemovePathPoint )( entity_, point_ );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetRepPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
MT_Vector2D* PathPointAdapter::GetRepPoint() const
{
    return const_cast< MT_Vector2D* >( &GetPos() );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetTypePoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
int PathPointAdapter::GetTypePoint() const
{
    return GET_HOOK( GetPathTypePoint )( point_ );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::IsAvantPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
bool PathPointAdapter::IsAvantPoint() const
{
    return GET_HOOK( GetPathTypePoint )( point_ ) == DEC_PathPoint::eTypePointFront;
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::IsPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
bool PathPointAdapter::IsPoint() const
{
    return GET_HOOK( GetPathTypePoint )( point_ ) == DEC_PathPoint::eTypePointSpecial;
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetDestPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
boost::shared_ptr< PathPointAdapter > PathPointAdapter::GetDestPoint() const
{
    return boost::make_shared< PathPointAdapter >( entity_, GET_HOOK( GetPathDestPoint )( point_ ) );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetTypeLimaPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
int PathPointAdapter::GetTypeLimaPoint() const
{
    return GET_HOOK( GetPathTypeLimaPoint )( point_ );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetLimaPoint
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
unsigned int PathPointAdapter::GetLimaPoint() const
{
    return GET_HOOK( GetPathLimaPoint )( point_ );
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetDIAType
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
const std::string& PathPointAdapter::GetDIAType() const
{
    type_ = GET_HOOK( GetPathDIAType )( point_ );
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PathPointAdapter::GetPos
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
const MT_Vector2D& PathPointAdapter::GetPos() const
{
    return *GET_HOOK( GetPathPos )( point_ );
}
