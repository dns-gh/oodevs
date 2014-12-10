// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SlopeDecelerationComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

// -----------------------------------------------------------------------------
// Name: SlopeDecelerationComputer constructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
SlopeDecelerationComputer::SlopeDecelerationComputer()
    : deceleration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SlopeDecelerationComputer destructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
SlopeDecelerationComputer::~SlopeDecelerationComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SlopeDecelerationComputer::ApplyOnComponent
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
void SlopeDecelerationComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    if( component.CanMove() )
        deceleration_ = std::max( deceleration_, component.GetType().GetSlopeDeceleration() );
}

// -----------------------------------------------------------------------------
// Name: SlopeDecelerationComputer::GetSlopeDeceleration
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
double SlopeDecelerationComputer::GetSlopeDeceleration() const
{
    return deceleration_;
}

}
