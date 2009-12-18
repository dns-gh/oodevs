// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/LocationComputerFactory.h"
#include "simulation_kernel/DefaultLocationComputer.h"

using namespace location;
// -----------------------------------------------------------------------------
// Name: LocationComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
LocationComputerFactory::LocationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
LocationComputerFactory::~LocationComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< LocationComputer_ABC > LocationComputerFactory::Create() const
{
    std::auto_ptr< LocationComputer_ABC > pLocationComputer( new DefaultLocationComputer() );
    return pLocationComputer;
}