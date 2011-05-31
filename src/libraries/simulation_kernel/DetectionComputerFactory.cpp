// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DetectionComputerFactory.h"
#include "simulation_kernel/DefaultDetectionComputer.h"
#include "simulation_kernel/DefaultPerceptionDistanceComputer.h"

using namespace detection;
// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DetectionComputerFactory::DetectionComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DetectionComputerFactory::~DetectionComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< DetectionComputer_ABC > DetectionComputerFactory::Create( MIL_Agent_ABC& target ) const
{
    return std::auto_ptr< DetectionComputer_ABC >( new DefaultDetectionComputer( target ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory Create
// Created: MGD 2009-10-06
// -----------------------------------------------------------------------------
std::auto_ptr< PerceptionDistanceComputer_ABC > DetectionComputerFactory::CreateDistanceComputer() const
{
    return std::auto_ptr< PerceptionDistanceComputer_ABC >( new DefaultPerceptionDistanceComputer() );
}