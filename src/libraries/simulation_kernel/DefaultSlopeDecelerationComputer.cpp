// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultSlopeDecelerationComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

// -----------------------------------------------------------------------------
// Name: DefaultSlopeDecelerationComputer constructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
DefaultSlopeDecelerationComputer::DefaultSlopeDecelerationComputer()
    : deceleration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultSlopeDecelerationComputer destructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
DefaultSlopeDecelerationComputer::~DefaultSlopeDecelerationComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultSlopeDecelerationComputer::ApplyOnComponent
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
void DefaultSlopeDecelerationComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    if( component.CanMove() )
        deceleration_ = std::max( deceleration_, component.GetType().GetSlopeDeceleration() );
}

// -----------------------------------------------------------------------------
// Name: DefaultSlopeDecelerationComputer::GetSlopeDeceleration
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
double DefaultSlopeDecelerationComputer::GetSlopeDeceleration() const
{
    return deceleration_;
}

}
