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

namespace
{
    double Square( double x )
    {
        return x * x;
    }
    const double pi = std::acos( -1. );
}

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
    rFactor_ = pathClass.GetEnemyCostOnContact() / 100.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent destructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::~DEC_Path_KnowledgeAgent()
{
    // NOTHING
}

namespace
{
    const double epsilon = 1 / pi; // in metres / pi
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeAgent::ComputeCost( const MT_Line& lineLink, const MT_Rect& boundingBox ) const
{
    if( !boundingBox.IsInside( vEnemyPosition_ ) )
        return 0;
    const MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const double rSqDistBtwUnitAndEnemy = vPositionProjection.SquareDistance( vEnemyPosition_ );
    if( rFactor_ > 0 && rSqDistBtwUnitAndEnemy < rSquareSecurityDistance_ )
        // Inverse-square law
        return rFactor_ / ( rSqDistBtwUnitAndEnemy + epsilon );
    return 0;
}
