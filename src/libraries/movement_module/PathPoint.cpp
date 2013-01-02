// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "PathPoint.h"
#include <tools/Exception.h>

using namespace sword;
using namespace sword::movement;

namespace
{
    std::size_t identifiers_ = 0;
}

//-----------------------------------------------------------------------------
// Name: PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
PathPoint::PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType )
    : identifier_( ++identifiers_ )
    , vPos_      ( vPos )
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
    : identifier_             ( ++identifiers_ )
    , vPos_                   ( vPos )
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
const boost::shared_ptr< PathPoint >& PathPoint::GetDestPoint() const
{
    throw MASA_EXCEPTION( "GetDestPoint cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetTypeLima
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
int PathPoint::GetTypeLima() const
{
    throw MASA_EXCEPTION( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetLimaID
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
unsigned int PathPoint::GetLimaID() const
{
    throw MASA_EXCEPTION( "GetLimaID cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetTypeTerrain
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
const TerrainData& PathPoint::GetTypeTerrain() const
{
    throw MASA_EXCEPTION( "GetTypeTerrain cannot be called for this Point class" );
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetPos
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
const MT_Vector2D& PathPoint::GetPos() const
{
    return vPos_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetType
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
PathPoint::E_Type PathPoint::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetObjectTypes
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& PathPoint::GetObjectTypes() const
{
    return nObjectTypes_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetObjectTypesToNextPoint
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
const TerrainData& PathPoint::GetObjectTypesToNextPoint() const
{
    return nObjectTypesToNextPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathPoint::GetID
// Created: SLI 2012-12-18
// -----------------------------------------------------------------------------
std::size_t PathPoint::GetID() const
{
    return identifier_;
}
