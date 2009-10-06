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
: pDetectionComputer_( new DefaultDetectionComputer() )
, pDistanceComputer_( new DefaultPerceptionDistanceComputer() )
{
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
DetectionComputer_ABC& DetectionComputerFactory::Create( MIL_Agent_ABC& target ) const
{
    pDetectionComputer_->Reset( target );
    return *pDetectionComputer_;
}

// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory Create
// Created: MGD 2009-10-06
// -----------------------------------------------------------------------------
PerceptionDistanceComputer_ABC& DetectionComputerFactory::CreateDistanceComputer() const
{
    pDistanceComputer_->Reset();
    return *pDistanceComputer_;
}