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
#include "MT_Tools/MT_Line.h"

namespace
{
    double ComputeSecurityDistance( double maxRangeToFire )
    {
        const double capped = std::min( 2000., std::max( 1000., maxRangeToFire ) );
        return capped * capped;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const MT_Vector2D& position, double enemyCostAtSecurityRange, double enemyCostOnContact, double maxRangeToFire )
    : vEnemyPosition_         ( position )
    , rSquareSecurityDistance_( ComputeSecurityDistance( maxRangeToFire ) )
    , rFactor_                ( enemyCostOnContact / 100. )
    , rOffset_                ( enemyCostAtSecurityRange )
{
    // NOTHING
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
    const double pi = std::acos( -1. );
    const double epsilon = 1; // in metres
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeAgent::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    const MT_Line lineLink( from, to );
    MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const double rSqDistBtwUnitAndEnemy = vPositionProjection.SquareDistance( vEnemyPosition_ );

    if( rFactor_ > 0 && rSqDistBtwUnitAndEnemy < rSquareSecurityDistance_ )
    {
        // Inverse-square law
        double intensity = rSquareSecurityDistance_ * rFactor_ / ( pi * rSqDistBtwUnitAndEnemy + epsilon );
        return rOffset_ * intensity;
    }
    return 0.;
}
