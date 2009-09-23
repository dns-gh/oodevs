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

using namespace detection;
// -----------------------------------------------------------------------------
// Name: DetectionComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DetectionComputerFactory::DetectionComputerFactory() 
: pDetectionComputer_( new DefaultDetectionComputer() )
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
// Name: DetectionComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
DetectionComputer_ABC& DetectionComputerFactory::Create( MIL_Agent_ABC& target ) const
{
    pDetectionComputer_->Reset( target );
    return *pDetectionComputer_;
}