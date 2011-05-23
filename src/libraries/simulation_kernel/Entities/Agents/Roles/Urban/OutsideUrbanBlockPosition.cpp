// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "OutsideUrbanBlockPosition.h"
#include "UrbanLocationComputer_ABC.h"
#include "MT_Tools/MT_Ellipse.h"

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition constructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
OutsideUrbanBlockPosition::OutsideUrbanBlockPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition destructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
OutsideUrbanBlockPosition::~OutsideUrbanBlockPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition::GetFirerPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
MT_Vector2D OutsideUrbanBlockPosition::GetFirerPosition( MIL_Agent_ABC& /*target*/, UrbanLocationComputer_ABC::Results& firerResult ) const
{
    return firerResult.position_;
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition::GetTargetPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
MT_Vector2D OutsideUrbanBlockPosition::GetTargetPosition( MIL_Agent_ABC& /*firer*/, UrbanLocationComputer_ABC::Results& targetResult ) const
{
    return targetResult.position_;
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
float OutsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const
{
    if( attritionSurface.IsInside( result.position_ ) )
        return 1.0;
    else
        return 0.0;
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
float OutsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const geometry::Polygon2f& polygon, float /*modificator*/ ) const
{
    if( polygon.IsInside( geometry::Point2f( result.position_.rX_, result.position_.rY_ ) ) )
        return 1.f;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: OutsideUrbanBlockPosition::ComputeUrbanProtection
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
double OutsideUrbanBlockPosition::ComputeUrbanProtection( const PHY_DotationCategory& /*dotationCategory*/ ) const
{
    return 0.;
}
