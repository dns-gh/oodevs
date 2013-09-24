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
{
    // NOTHING
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
std::auto_ptr< PostureComputer_ABC > PostureComputerFactory::Create( const MIL_Random_ABC& random, const PostureTime_ABC& time, const PHY_Posture& previousPosture, const PHY_Posture& currentPosture,
                                                         bool bIsDead, bool bDiscreteModeEnabled, double rCompletionPercentage, double rStealthFactor,
                                                         double rTimingFactor, bool isParkedOnEngineerArea ) const
{
    return std::auto_ptr< PostureComputer_ABC >( new DefaultPostureComputer( random, time, previousPosture, currentPosture, bIsDead, bDiscreteModeEnabled, rCompletionPercentage,
                                                                             rStealthFactor, rTimingFactor, isParkedOnEngineerArea ) );
}
