// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/WeaponAvailabilityComputerFactory.h"
#include "simulation_kernel/DefaultWeaponAvailabilityComputer.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: WeaponAvailabilityComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
WeaponAvailabilityComputerFactory::WeaponAvailabilityComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponAvailabilityComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
WeaponAvailabilityComputerFactory::~WeaponAvailabilityComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponAvailabilityComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< WeaponAvailabilityComputer_ABC > WeaponAvailabilityComputerFactory::Create( FireData_ABC& firerWeapons ) const
{
    return std::auto_ptr< WeaponAvailabilityComputer_ABC >( new DefaultWeaponAvailabilityComputer( firerWeapons ) );
}
