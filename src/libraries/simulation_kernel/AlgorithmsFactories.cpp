// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/WeaponAvailabilityComputerFactory.h"
#include "simulation_kernel/ComposantesAbleToBeFiredComputerFactory.h"
#include "simulation_kernel/LocationComputerFactory.h"
#include "simulation_kernel/PostureComputerFactory.h"

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::AlgorithmsFactories()
    : weaponAvailabilityComputerFactory_( new firing::WeaponAvailabilityComputerFactory() )
    , composantesAbleToBeFiredComputerFactory_( new firing::ComposantesAbleToBeFiredComputerFactory() )
    , locationComputerFactory_( new location::LocationComputerFactory() )
    , postureComputerFactory_( new posture::PostureComputerFactory() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AlgorithmsFactories destructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AlgorithmsFactories::~AlgorithmsFactories()
{
    // NOTHING
}