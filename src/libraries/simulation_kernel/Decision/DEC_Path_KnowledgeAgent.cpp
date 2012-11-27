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
    double Square( double x )
    {
        return x * x;
    }
    const double pi = std::acos( -1. );
}

const double DEC_Path_KnowledgeAgent::maxFireDistance_ = 2000;

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const MT_Vector2D& position, double enemyCostAtSecurityRange, double enemyCostOnContact, double maxRangeToFire )
    : vEnemyPosition_         ( position )
    , rSquareSecurityDistance_( Square( std::min( maxFireDistance_, std::max( 1000., maxRangeToFire ) ) ) )
    , rFactor_                ( std::max( 0., enemyCostAtSecurityRange * rSquareSecurityDistance_ * enemyCostOnContact / 100 / pi ) )
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
    const double epsilon = 1 / pi; // in metres / pi
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// Modified: CMA 2011-04-27
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeAgent::ComputeCost( const MT_Line& lineLink, const BoundingBox& box ) const
{
    if( box.IsOutside( vEnemyPosition_ ) )
        return 0;
    const MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vEnemyPosition_ );
    const double rSqDistBtwUnitAndEnemy = vPositionProjection.SquareDistance( vEnemyPosition_ );
    if( rSqDistBtwUnitAndEnemy >= rSquareSecurityDistance_ )
        return 0;
    // Inverse-square law
    return rFactor_ / ( rSqDistBtwUnitAndEnemy + epsilon );
}
