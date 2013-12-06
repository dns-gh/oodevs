// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DetectionComputer.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"

using namespace detection;

// -----------------------------------------------------------------------------
// Name: DetectionComputer::DetectionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DetectionComputer::DetectionComputer( MIL_Agent_ABC& target )
    : pTarget_       ( &target )
    , bAlreadySeen_  ( false )
    , bIsStealth_    ( false )
    , bIsUnderground_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::~DetectionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DetectionComputer::~DetectionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::~DetectionComputer
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DetectionComputer::GetTarget()
{
    return *pTarget_;
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::AlreadyPerceived
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
void DetectionComputer::AlreadyPerceived()
{
    bAlreadySeen_ = true;
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::NotifyStealth
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
void DetectionComputer::NotifyStealth()
{
    bIsStealth_ = true;
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::SetUnderground
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void DetectionComputer::SetUnderground( bool underground )
{
    bIsUnderground_ = underground;
}

// -----------------------------------------------------------------------------
// Name: DetectionComputer::CanBeSeen
// Created: MGD 2009-09-23
// -----------------------------------------------------------------------------
bool DetectionComputer::CanBeSeen()
{
    if( bIsUnderground_ )
        return false;
    return bAlreadySeen_ || !bIsStealth_;
}
