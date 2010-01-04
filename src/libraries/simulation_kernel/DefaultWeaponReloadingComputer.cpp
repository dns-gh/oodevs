// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/DefaultWeaponReloadingComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include <numeric>

using namespace firing;

// -----------------------------------------------------------------------------
// Name: DefaultWeaponReloadingComputer::DefaultWeaponReloadingComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultWeaponReloadingComputer::DefaultWeaponReloadingComputer( double initialDuration )
    : initialDuration_( initialDuration )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultWeaponReloadingComputer::~DefaultWeaponReloadingComputer
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
DefaultWeaponReloadingComputer::~DefaultWeaponReloadingComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultWeaponReloadingComputer::AddModifier
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void DefaultWeaponReloadingComputer::AddModifier( double modifier )
{
    modifiers_.push_back( modifier );
}

// -----------------------------------------------------------------------------
// Name: DefaultWeaponReloadingComputer::GetDuration
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
double DefaultWeaponReloadingComputer::GetDuration() const
{
    return std::accumulate( modifiers_.begin(), modifiers_.end(), 1., std::multiplies< double >() );
}
