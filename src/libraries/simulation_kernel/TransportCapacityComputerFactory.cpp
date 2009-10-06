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

namespace transport {

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportComputerFactory::TransportComputerFactory() :
    capacityComputer_(new DefaultTransportCapacityComputer()),
    weightComputer_(new DefaultTransportWeightComputer()),
    permissionComputer_(new DefaultTransportPermissionComputer())
{

}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportComputerFactory::~TransportComputerFactory()
{
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreateCapacityComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportCapacityComputer_ABC& TransportComputerFactory::CreateCapacityComputer( ) const
{
    capacityComputer_->Reset();
    return *capacityComputer_;
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreateWeightComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportWeightComputer_ABC& TransportComputerFactory::CreateWeightComputer(const TransportStrategy_ABC* st) const
{
    weightComputer_->Reset(st);
    return *weightComputer_;
}

// -----------------------------------------------------------------------------
// Name: TransportComputerFactory::CreatePermissionComputer
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
TransportPermissionComputer_ABC& TransportComputerFactory::CreatePermissionComputer() const
{
    permissionComputer_->Reset();
    return *permissionComputer_;
}

}
