// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathPoint.h"

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType )
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
// Name: DEC_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool waypoint )
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
// Name: DEC_PathPoint destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::~DEC_PathPoint()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::IsInObject
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_PathPoint::IsInObject( const TerrainData& data ) const
{
    return nObjectTypes_.ContainsAll( data );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::WillBeInObject
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
bool DEC_PathPoint::WillBeInObject( const TerrainData& data ) const
{
    return nObjectTypesToNextPoint_.ContainsAll( data );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void DEC_PathPoint::SendToDIA( DEC_Representations& /*agent*/, boost::shared_ptr< DEC_PathPoint > /*point*/ ) const
{
    throw MASA_EXCEPTION( "SendToDIA cannot be called for this Point class" );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::SendToDIA
// Created: MGD 10-03-11
//-----------------------------------------------------------------------------
void DEC_PathPoint::RemoveFromDIA( boost::shared_ptr< DEC_PathPoint > self )
{
    throw MASA_EXCEPTION( "RemoveFromDIA cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_PathPoint::GetDIAType() const
{
    return diaType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetTypePoint
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
DEC_PathPoint::E_TypePoint DEC_PathPoint::GetTypePoint() const
{
    return nPointType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetDestPoint
// Created: MGD 2010-03-10 @TODO MGD Refactor PathPoint hierarchy to avoid this ugly
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_PathPoint > DEC_PathPoint::GetDestPoint() const
{
    throw MASA_EXCEPTION( "GetDestPoint cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetTypeLima
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
int DEC_PathPoint::GetTypeLima()
{
    throw MASA_EXCEPTION( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetLimaID
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
unsigned int DEC_PathPoint::GetLimaID()
{
    throw MASA_EXCEPTION( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetTypeTerrain
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
const TerrainData& DEC_PathPoint::GetTypeTerrain() const
{
    throw MASA_EXCEPTION( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::IsSlopeValid
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
bool DEC_PathPoint::IsSlopeValid() const
{
    return slope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::IsWaypoint
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
bool DEC_PathPoint::IsWaypoint() const
{
    return waypoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::IsPartial
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
bool DEC_PathPoint::IsPartial() const
{
    return partial_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::NotifyPartial
// Created: LGY 2014-04-03
// -----------------------------------------------------------------------------
void DEC_PathPoint::NotifyPartial()
{
    partial_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
double DEC_PathPoint::GetSlope() const
{
    if( !slope_ )
        throw MASA_EXCEPTION( "Accessing invalid slope" );
    return *slope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::SetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
void DEC_PathPoint::SetSlope( double slope )
{
    slope_ = slope;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetPos
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const MT_Vector2D& DEC_PathPoint::GetPos() const
{
    return vPos_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetType
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
DEC_PathPoint::E_Type DEC_PathPoint::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetObjectTypes
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& DEC_PathPoint::GetObjectTypes() const
{
    return nObjectTypes_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetObjectTypesToNextPoint
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& DEC_PathPoint::GetObjectTypesToNextPoint() const
{
    return nObjectTypesToNextPoint_;
}
