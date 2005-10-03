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

#include "MIL_pch.h"
#include "DEC_PathPoint.h"


//-----------------------------------------------------------------------------
// Name: DEC_PathPoint constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const MT_Vector2D& vPos )
    : vPos_                   ( vPos )
    , nType_                  ( eTypePointSpecial )
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
    , nObjectTypes_           ( nObjectTypes )
    , nObjectTypesToNextPoint_( nObjectTypesToNextPoint )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint constructor
// Created: JDY 03-02-19
//-----------------------------------------------------------------------------
DEC_PathPoint::DEC_PathPoint( const DEC_PathPoint& pathPoint )
    : vPos_(pathPoint.vPos_)
    , nType_(pathPoint.nType_)
    , nObjectTypes_(pathPoint.nObjectTypes_ )
    , nObjectTypesToNextPoint_(pathPoint.nObjectTypesToNextPoint_)
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
void DEC_PathPoint::SendToDIA( DEC_RolePion_Decision& /*agent*/ ) const
{
    assert( false );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::RemoveFromDIA
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
void DEC_PathPoint::RemoveFromDIA( DEC_RolePion_Decision& /*agent*/ ) const
{
    
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
