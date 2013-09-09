// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultPerceptionDistanceComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace detection
{

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::DefaultPerceptionDistanceComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultPerceptionDistanceComputer::DefaultPerceptionDistanceComputer()
    : factor_( 1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::~DefaultPerceptionDistanceComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultPerceptionDistanceComputer::~DefaultPerceptionDistanceComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::AddModifier
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void DefaultPerceptionDistanceComputer::AddModifier( double modifier )
{
    if( modifier != 0. )
        factor_ *= modifier;
}

// -----------------------------------------------------------------------------
// Name: DefaultPerceptionDistanceComputer::GetDuration
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
double DefaultPerceptionDistanceComputer::GetFactor() const
{
    return factor_;
}

} // namespace detection
