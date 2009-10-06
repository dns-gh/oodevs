// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/WeaponReloadingComputerFactory.h"
#include "simulation_kernel/DefaultWeaponReloadingComputer.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: WeaponReloadingComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
WeaponReloadingComputerFactory::WeaponReloadingComputerFactory()
: pComputer_( new DefaultWeaponReloadingComputer() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponReloadingComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
WeaponReloadingComputerFactory::~WeaponReloadingComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeaponReloadingComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
WeaponReloadingComputer_ABC& WeaponReloadingComputerFactory::Create( double duration ) const
{
    pComputer_->Reset( duration );
    return *pComputer_;
}