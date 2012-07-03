// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "PathPoint.h"

using namespace sword;
using namespace sword::movement;

//-----------------------------------------------------------------------------
// Name: PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
PathPoint::PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType )
    : vPos_      ( vPos )
    , nType_     ( type )
    , nPointType_( nPointType )
    , diaType_   ( szDIARepType )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
PathPoint::PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
    : vPos_                   ( vPos )
    , nType_                  ( eTypePointPath )
    , nPointType_             ( eTypePointNormal )
    , nObjectTypes_           ( nObjectTypes )
    , nObjectTypesToNextPoint_( nObjectTypesToNextPoint )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PathPoint destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
PathPoint::~PathPoint()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PathPoint::IsInObject
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool PathPoint::IsInObject( const TerrainData& data ) const
{
    return nObjectTypes_.ContainsAll( data );
}

//-----------------------------------------------------------------------------
// Name: PathPoint::WillBeInObject
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
bool PathPoint::WillBeInObject( const TerrainData& data ) const
{
    return nObjectTypesToNextPoint_.ContainsAll( data );
}

//-----------------------------------------------------------------------------
// Name: PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void PathPoint::SendToDIA( unsigned int /*entity*/, boost::shared_ptr< PathPoint > /*point*/ ) const
{
    throw std::runtime_error( "SendToDIA cannot be called for this Point class" );
}

//-----------------------------------------------------------------------------
// Name: PathPoint::SendToDIA
// Created: MGD 10-03-11
//-----------------------------------------------------------------------------
void PathPoint::RemoveFromDIA( boost::shared_ptr< PathPoint > self )
{
    throw std::runtime_error( "RemoveFromDIA cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& PathPoint::GetDIAType() const
{
    return diaType_;
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetTypePoint
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
PathPoint::E_TypePoint PathPoint::GetTypePoint() const
{
    return nPointType_;
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetDestPoint
// Created: MGD 2010-03-10 @TODO MGD Refactor PathPoint hierarchy to avoid this ugly
// -----------------------------------------------------------------------------
boost::shared_ptr< PathPoint > PathPoint::GetDestPoint() const
{
    throw std::runtime_error( "GetDestPoint cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetTypeLima
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
int PathPoint::GetTypeLima()
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetLimaID
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
unsigned int PathPoint::GetLimaID()
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetTypeTerrain
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
const TerrainData& PathPoint::GetTypeTerrain() const
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}
