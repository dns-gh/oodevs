//*****************************************************************************
//
// $Created: JVT 02-12-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathPoint.cpp $
// $Author: Nld $
// $Modtime: 1/04/05 9:45 $
// $Revision: 5 $
// $Workfile: DEC_PathPoint.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathPoint.h"


//-----------------------------------------------------------------------------
// Name: DEC_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType )
    : vPos_                   ( vPos )
    , nType_                  ( type )
    , nPointType_             ( nPointType )
    , diaType_                ( szDIARepType )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
    : vPos_                   ( vPos )
    , nType_                  ( eTypePointPath )
    , nPointType_             ( eTypePointNormal )
    , nObjectTypes_           ( nObjectTypes )
    , nObjectTypesToNextPoint_( nObjectTypesToNextPoint )
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
    throw std::runtime_error( "SendToDIA cannot be called for this Point class" );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::SendToDIA
// Created: MGD 10-03-11
//-----------------------------------------------------------------------------
void DEC_PathPoint::RemoveFromDIA( boost::shared_ptr< DEC_PathPoint > self )
{
    throw std::runtime_error( "RemoveFromDIA cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_PathPoint::Dump() const
{
    std::cout << "    PathPoint " << vPos_
              << " - Type : " << nObjectTypes_.DumpToString()
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString()
              << std::endl;
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
    throw std::runtime_error( "GetDestPoint cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetTypeLima
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
int DEC_PathPoint::GetTypeLima()
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetLimaID
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
unsigned int DEC_PathPoint::GetLimaID()
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetTypeTerrain
// Created: MGD 2010-03-10
// -----------------------------------------------------------------------------
const TerrainData& DEC_PathPoint::GetTypeTerrain() const
{
    throw std::runtime_error( "GetTypeLima cannot be called for this Point class" );
}
