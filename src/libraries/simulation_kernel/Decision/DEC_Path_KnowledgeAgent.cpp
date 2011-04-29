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
#include "DEC_PerceptionFunctions.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Agent& knowledge, const MIL_Agent_ABC& pion )
    : vEnemyPosition_ ( knowledge.GetPosition() )
    , pKnowledgeAgent_( &knowledge.GetAgentKnown() )
{
    assert( pKnowledgeAgent_ );

    // Range between 1000 and 2000 meters
    double maxRangeToFire = knowledge.GetMaxRangeToFireOn( pion, 0 );
    if( maxRangeToFire > 2000. )
        maxRangeToFire = 2000.;
    else if( maxRangeToFire < 1000. )
        maxRangeToFire = 1000.;

    rSquareSecurityDistance_ = maxRangeToFire * maxRangeToFire;
    rOffset_ = pathClass.GetEnemyCostAtSecurityRange();
    if( pathClass.GetEnemyCostOnContact() > 0. )
        rFactor_ = 100. / pathClass.GetEnemyCostOnContact();
    else
        rFactor_ = 0.;
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
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeAgent::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    static const double pi = std::acos( -1. );

    const MT_Line lineLink( from, to );
    MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const double rSqDistBtwUnitAndEnemy = vPositionProjection.SquareDistance( vEnemyPosition_ );

    if( ( rFactor_ > 0. ) &&
        ( rSqDistBtwUnitAndEnemy < rSquareSecurityDistance_ ) &&
        DEC_PerceptionFunctions::IsPointVisible( *pKnowledgeAgent_, &vPositionProjection ) )
    {
        // Inverse-square law
        double intensity = rSquareSecurityDistance_ / ( rFactor_ * pi * rSqDistBtwUnitAndEnemy );
        return rOffset_ * intensity;
    }
    return 0.;
}
