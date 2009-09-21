// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/PostureComputerFactory.h"
#include "simulation_kernel/DefaultPostureComputer.h"

using namespace posture;
// -----------------------------------------------------------------------------
// Name: PostureComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
PostureComputerFactory::PostureComputerFactory() 
: pPostureComputer_( new DefaultPostureComputer() )
{
}

// -----------------------------------------------------------------------------
// Name: PostureComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
PostureComputerFactory::~PostureComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PostureComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
PostureComputer_ABC& PostureComputerFactory::Create( const PHY_Posture& posture, bool bIsLoaded, bool bDiscreteModeEnabled ) const
{
    pPostureComputer_->Reset( posture, bIsLoaded, bDiscreteModeEnabled );
    return *pPostureComputer_;
}