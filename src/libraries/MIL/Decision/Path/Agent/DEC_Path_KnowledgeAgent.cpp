// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_KnowledgeAgent.cpp $
// $Author: Age $
// $Modtime: 8/03/05 18:54 $
// $Revision: 8 $
// $Workfile: DEC_Path_KnowledgeAgent.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Path_KnowledgeAgent.h"

#include "DEC_PathClass.h"
#include "Knowledge/DEC_Knowledge_Agent.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const DEC_PathClass& pathClass, const DEC_Knowledge_Agent& knowledge, const MIL_AgentPion& pion )
    : vEnemyPosition_   ( knowledge.GetPosition() )
    , rSecurityDistance_( knowledge.GetMaxRangeToFireOn( pion, 0 ) )
{ 
    if( rSecurityDistance_ < 1000. )
        rSecurityDistance_ = 1000.;

    rFactor_ = ( pathClass.GetEnemyCostAtSecurityRange() - pathClass.GetEnemyCostOnContact() ) / rSecurityDistance_;
    rOffset_ = pathClass.GetEnemyCostOnContact();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent destructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::~DEC_Path_KnowledgeAgent()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
MT_Float DEC_Path_KnowledgeAgent::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    const MT_Line lineLink( from, to );
    const MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const MT_Float rDistBtwUnitAndEnemy = vPositionProjection.Distance( vEnemyPosition_  );

    if( rDistBtwUnitAndEnemy > rSecurityDistance_ )
        return 0.;
    return rFactor_ * rDistBtwUnitAndEnemy + rOffset_;    
}
