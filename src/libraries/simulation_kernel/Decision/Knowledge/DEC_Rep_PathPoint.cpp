//*****************************************************************************
// 
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 6 $
// $Workfile: DEC_Rep_PathPoint.cpp $
// 
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Rep_PathPoint.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : DEC_PathPoint     ( vPos, szDIARepType )
    , nTypePoint_       ( nTypePoint )
    , nTypeTerrain_     ( nTypeTerrain )
    , pSentToDiaAgent_  ( 0 )
{
    assert( nType_ != eTypePointPath );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::~DEC_Rep_PathPoint()
{
    if( pSentToDiaAgent_ )
    {
        pSentToDiaAgent_->RemoveFromPointsCategory( const_cast< DEC_Rep_PathPoint* >( this ) );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::SendToDIA( DEC_Representations& agent ) const
{
    if( pSentToDiaAgent_ )
        return;
        
    // ATTENTION, si cette fonction est appelée, alors l'agent physique s'est automatiquement arrêté sur la position du point...
    agent.AddToPointsCategory( const_cast< DEC_Rep_PathPoint* >( this ) );

    pSentToDiaAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint::Dump() const
{
    std::cout << "    Rep_PathPoint " << vPos_ 
              << " - Type : " << nObjectTypes_.DumpToString() 
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString() 
              << " - Type terrain " << nTypeTerrain_.DumpToString() 
              << " - nTypePoint_ " << (uint)nTypePoint_
              << std::endl;
}

