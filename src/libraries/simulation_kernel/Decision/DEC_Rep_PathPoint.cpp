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
#include <boost/make_shared.hpp>

DEC_DIA_PathPoint::DEC_DIA_PathPoint( const MT_Vector2D& vPos, E_Type type,
        E_TypePoint nTypePoint, const char* szDIARepType )
    : DEC_PathPoint     ( vPos, type, nTypePoint, szDIARepType )
    , pSentToDiaAgent_  ( 0 )
{
    // NOTHING
}

void DEC_DIA_PathPoint::SendToDIA( DEC_Representations& agent,
        boost::shared_ptr< DEC_DIA_PathPoint > point ) const
{
    if( pSentToDiaAgent_ )
        return;
    agent.AddToPointsCategory( point );
    pSentToDiaAgent_ = &agent;
}

void DEC_DIA_PathPoint::RemoveFromDIA( boost::shared_ptr< DEC_DIA_PathPoint > self )
{
    if( pSentToDiaAgent_ )
        pSentToDiaAgent_->RemoveFromPointsCategory( self );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : DEC_DIA_PathPoint( vPos, eTypePointSpecial, nTypePoint, szDIARepType )
    , nTypeTerrain_( nTypeTerrain )
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
// Name: DEC_Rep_PathPoint::GetTypeTerrain
// Created: AGN 03-03-25
//-----------------------------------------------------------------------------
const TerrainData& DEC_Rep_PathPoint::GetTypeTerrain() const
{
    return nTypeTerrain_;
}

boost::shared_ptr< DEC_Rep_PathPoint > CreateSpecialPoint(
        const MT_Vector2D& pos, const TerrainData& terrain )
{
    return boost::make_shared< DEC_Rep_PathPoint >( pos,
        DEC_Rep_PathPoint::eTypePointParticulier, terrain, "Rep_PointParticulier" );
}
