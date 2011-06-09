// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultDetectionComputer.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"

using namespace detection;

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::DefaultDetectionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultDetectionComputer::DefaultDetectionComputer( MIL_Agent_ABC& target )
    : pTarget_       ( &target )
    , bAlreadySeen_  ( false )
    , bIsStealth_    ( false )
    , bIsUnderground_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::~DefaultDetectionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultDetectionComputer::~DefaultDetectionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::~DefaultDetectionComputer
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DefaultDetectionComputer::GetTarget()
{
    return *pTarget_;
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::AlreadyPerceived
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
void DefaultDetectionComputer::AlreadyPerceived()
{
    bAlreadySeen_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::NotifyStealth
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
void DefaultDetectionComputer::NotifyStealth()
{
    bIsStealth_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::SetUnderground
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void DefaultDetectionComputer::SetUnderground( bool underground )
{
    bIsUnderground_ = underground;
}

// -----------------------------------------------------------------------------
// Name: DefaultDetectionComputer::CanBeSeen
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
bool DefaultDetectionComputer::CanBeSeen()
{
    return ( bAlreadySeen_ || ( !bIsUnderground_ && !bIsStealth_ ) );
}
