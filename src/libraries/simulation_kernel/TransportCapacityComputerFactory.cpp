// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TransportCapacityComputerFactory.h"
#include "DefaultTransportCapacityComputer.h"
#include "DefaultTransportWeightComputer.h"
#include "DefaultTransportPermissionComputer.h"

using namespace transport;

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportComputerFactory::TransportComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportComputerFactory::~TransportComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreateCapacityComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
std::auto_ptr< TransportCapacityComputer_ABC > TransportComputerFactory::CreateCapacityComputer( ) const
{
    return std::auto_ptr< TransportCapacityComputer_ABC >( new DefaultTransportCapacityComputer() );
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreateWeightComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
std::auto_ptr< TransportWeightComputer_ABC > TransportComputerFactory::CreateWeightComputer( const TransportStrategy_ABC* strategy ) const
{
    return std::auto_ptr< TransportWeightComputer_ABC >( new DefaultTransportWeightComputer( strategy ) );
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreatePermissionComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
std::auto_ptr< TransportPermissionComputer_ABC > TransportComputerFactory::CreatePermissionComputer() const
{
    return std::auto_ptr< TransportPermissionComputer_ABC >( new DefaultTransportPermissionComputer() );
}
