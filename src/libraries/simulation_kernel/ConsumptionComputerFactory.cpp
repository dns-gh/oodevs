// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/ConsumptionComputerFactory.h"
#include "simulation_kernel/DefaultConsumptionComputer.h"
#include "simulation_kernel/DefaultConsumptionModeChangeRequest.h"

using namespace dotation;
// -----------------------------------------------------------------------------
// Name: ConsumptionComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
ConsumptionComputerFactory::ConsumptionComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsumptionComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
ConsumptionComputerFactory::~ConsumptionComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsumptionComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< ConsumptionComputer_ABC > ConsumptionComputerFactory::CreateConsumptionComputer() const
{
    return std::auto_ptr< ConsumptionComputer_ABC >( new DefaultConsumptionComputer() );
}

// -----------------------------------------------------------------------------
// Name: ConsumptionComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< ConsumptionModeChangeRequest_ABC > ConsumptionComputerFactory::CreateConsumptionModeChangeRequest( const PHY_ConsumptionType& newMode ) const
{
    return std::auto_ptr< ConsumptionModeChangeRequest_ABC >( new DefaultConsumptionModeChangeRequest( newMode ) );
}
