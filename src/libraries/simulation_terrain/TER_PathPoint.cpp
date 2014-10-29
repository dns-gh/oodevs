// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathPoint.h"

//-----------------------------------------------------------------------------
// Name: TER_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
TER_PathPoint::TER_PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType )
    : vPos_( vPos )
    , nType_( type )
    , nPointType_( nPointType )
    , diaType_( szDIARepType )
    , waypoint_( false )
    , partial_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
TER_PathPoint::TER_PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool waypoint )
    : vPos_( vPos )
    , nType_( eTypePointPath )
    , nPointType_( eTypePointNormal )
    , nObjectTypes_( nObjectTypes )
    , nObjectTypesToNextPoint_( nObjectTypesToNextPoint )
    , waypoint_( waypoint )
    , partial_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
TER_PathPoint::~TER_PathPoint()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::IsInObject
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool TER_PathPoint::IsInObject( const TerrainData& data ) const
{
    return nObjectTypes_.ContainsAll( data );
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::WillBeInObject
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
bool TER_PathPoint::WillBeInObject( const TerrainData& data ) const
{
    return nObjectTypesToNextPoint_.ContainsAll( data );
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& TER_PathPoint::GetDIAType() const
{
    return diaType_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::GetTypePoint
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
TER_PathPoint::E_TypePoint TER_PathPoint::GetTypePoint() const
{
    return nPointType_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::IsSlopeValid
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
bool TER_PathPoint::IsSlopeValid() const
{
    return slope_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::IsWaypoint
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
bool TER_PathPoint::IsWaypoint() const
{
    return waypoint_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::IsPartial
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
bool TER_PathPoint::IsPartial() const
{
    return partial_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::NotifyPartial
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
void TER_PathPoint::NotifyPartial()
{
    partial_ = true;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::NotifyWaypoint
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
void TER_PathPoint::NotifyWaypoint()
{
    waypoint_ = true;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::GetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
double TER_PathPoint::GetSlope() const
{
    if( !slope_ )
        throw MASA_EXCEPTION( "Accessing invalid slope" );
    return *slope_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathPoint::SetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
void TER_PathPoint::SetSlope( double slope )
{
    slope_ = slope;
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::GetPos
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const MT_Vector2D& TER_PathPoint::GetPos() const
{
    return vPos_;
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::GetType
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
TER_PathPoint::E_Type TER_PathPoint::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::GetObjectTypes
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& TER_PathPoint::GetObjectTypes() const
{
    return nObjectTypes_;
}

//-----------------------------------------------------------------------------
// Name: TER_PathPoint::GetObjectTypesToNextPoint
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& TER_PathPoint::GetObjectTypesToNextPoint() const
{
    return nObjectTypesToNextPoint_;
}
