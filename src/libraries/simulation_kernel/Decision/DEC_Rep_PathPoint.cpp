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
#include "Decision/DEC_Representations.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : DEC_PathPoint     ( vPos, eTypePointSpecial, nTypePoint, szDIARepType )
    , nTypeTerrain_     ( nTypeTerrain )
    , pSentToDiaAgent_  ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::~DEC_Rep_PathPoint()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::SendToDIA( DEC_Representations& agent, boost::shared_ptr< DEC_PathPoint > point ) const
{
    if( pSentToDiaAgent_ )
        return;
    // ATTENTION, si cette fonction est appelée, alors l'agent physique s'est automatiquement arrêté sur la position du point...
    agent.AddToPointsCategory( point );
    pSentToDiaAgent_ = &agent;
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::SendToDIA
// Created: MGD 10-03-11
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::RemoveFromDIA( boost::shared_ptr< DEC_PathPoint > self )
{
    if( pSentToDiaAgent_ )
        pSentToDiaAgent_->RemoveFromPointsCategory( self );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::GetTypeTerrain
// Created: AGN 03-03-25
//-----------------------------------------------------------------------------
const TerrainData& DEC_Rep_PathPoint::GetTypeTerrain() const
{
    return nTypeTerrain_;
}
