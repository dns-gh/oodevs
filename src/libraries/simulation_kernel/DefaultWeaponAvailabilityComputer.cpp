// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultWeaponAvailabilityComputer.h"
#include "simulation_kernel/FireData_ABC.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: DefaultWeaponAvailabilityComputer::DefaultWeaponAvailabilityComputer
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
DefaultWeaponAvailabilityComputer::DefaultWeaponAvailabilityComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultWeaponAvailabilityComputer::~DefaultWeaponAvailabilityComputer
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
DefaultWeaponAvailabilityComputer::~DefaultWeaponAvailabilityComputer()
{

}


// -----------------------------------------------------------------------------
// Name: DefaultWeaponAvailabilityComputer::Reset
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void DefaultWeaponAvailabilityComputer::Reset( FireData_ABC& firerWeapons )
{
    pFirerWeapons_ = &firerWeapons;
}

// -----------------------------------------------------------------------------
// Name: DefaultWeaponAvailabilityComputer::Execute
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
void DefaultWeaponAvailabilityComputer::ApplyOnWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon )
{
    (*pFirerWeapons_)( firer, weapon );
}
