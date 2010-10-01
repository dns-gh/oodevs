// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Agent_PathClass.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "MT_Tools/MT_Line.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Agent& knowledge, const MIL_Agent_ABC& pion )
    : vEnemyPosition_   ( knowledge.GetPosition() )
    , rSecurityDistance_( knowledge.GetMaxRangeToFireOn( pion, 0 ) )
{
    if( rSecurityDistance_ < 1000. )
        rSecurityDistance_ = 1000.;
    rFactor_ = ( pathClass.GetEnemyCostOnContact() - pathClass.GetEnemyCostAtSecurityRange() ) / rSecurityDistance_;
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

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeAgent::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    const MT_Line lineLink( from, to );
    const MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const double rDistBtwUnitAndEnemy = vPositionProjection.Distance( vEnemyPosition_ );
    if( rDistBtwUnitAndEnemy > rSecurityDistance_ )
        return 0.;
    return rOffset_ - rFactor_ * rDistBtwUnitAndEnemy;
}
