//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Front.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 4 $
// $Workfile: DEC_Rep_PathPoint_Front.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_Rep_PathPoint_Front.h"

#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::DEC_Rep_PathPoint_Front( const MT_Vector2D& vPos, DEC_Rep_PathPoint& dest )
    : DEC_PathPoint     ( vPos, "Rep_AvantPoint" )
    , destPoint_        ( dest )
    , pSentToDiaAgent_  ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::~DEC_Rep_PathPoint_Front()
{
    if( pSentToDiaAgent_ )
    {
        pSentToDiaAgent_->RemoveFromPointsCategory( const_cast< DEC_Rep_PathPoint_Front* >( this ) );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::SendToDIA
// Created: JVT 02-12-09
// Last modified: JVT 02-12-16
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::SendToDIA( DEC_Representations& agent ) const
{
    if( pSentToDiaAgent_ )
        return;

    agent.AddToPointsCategory( const_cast< DEC_Rep_PathPoint_Front* >( this ) );

    pSentToDiaAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
DEC_PathPoint& DEC_Rep_PathPoint_Front::GetDestPoint() const
{
    return destPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::Dump() const
{
    std::cout << "    DEC_Rep_PathPoint_Front " << vPos_ 
              << " - Type : " << nObjectTypes_.DumpToString() 
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString() 
              << " - DestPoint : " << destPoint_.GetPos()
              << " - Dest Type : " << destPoint_.GetTypeTerrain().DumpToString() 
              << std::endl;
}

