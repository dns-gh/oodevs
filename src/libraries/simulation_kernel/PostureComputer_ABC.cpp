// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/PostureComputer_ABC.h"

using namespace posture;

// -----------------------------------------------------------------------------
// Name: PostureComputer_ABC::PostureComputer_ABC::Parameters
// Created: MGD 2009-09-16
// -----------------------------------------------------------------------------
PostureComputer_ABC::Parameters::Parameters( const PHY_UnitType& unitType, const PHY_Posture& posture )
    : unitType_( unitType )
    , posture_( posture )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PostureComputer_ABC::PostureComputer_ABC::Results
// Created: LDC 2009-12-17
// -----------------------------------------------------------------------------
PostureComputer_ABC::Results::Results( double completion )
    : newPosture_( 0 )
    , postureCompletionPercentage_( completion )
    , bIsStealth_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PostureComputer_ABC::PostureComputer_ABC
// Created: MGD 2009-09-16
// -----------------------------------------------------------------------------
PostureComputer_ABC::PostureComputer_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PostureComputer_ABC::~PostureComputer_ABC
// Created: MGD 2009-09-16
// -----------------------------------------------------------------------------
PostureComputer_ABC::~PostureComputer_ABC()
{
    // NOTHING
}