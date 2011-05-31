// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/HealComputerFactory.h"
#include "simulation_kernel/DefaultHealComputer.h"

using namespace human;

// -----------------------------------------------------------------------------
// Name: HealableComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
HealComputerFactory::HealComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HealableComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
HealComputerFactory::~HealComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HealableComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< HealComputer_ABC > HealComputerFactory::Create() const
{
    return std::auto_ptr< HealComputer_ABC >( new DefaultHealComputer() );
}
