// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/DefaultMagicMoveComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"

using namespace moving;

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::DefaultMagicMoveComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultMagicMoveComputer::DefaultMagicMoveComputer()
    : bCanMagicMove_( true )
    , isSurrendered_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::~DefaultMagicMoveComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultMagicMoveComputer::~DefaultMagicMoveComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::NotifyReinforcing
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultMagicMoveComputer::NotifyReinforcing()
{
    bCanMagicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::NotifyTransported
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMagicMoveComputer::NotifyTransported()
{
    bCanMagicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::NotifySurrendered
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMagicMoveComputer::NotifySurrendered()
{
    isSurrendered_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::NotifyCannotFly
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMagicMoveComputer::NotifyCannotFly()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::CanMove
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
bool DefaultMagicMoveComputer::CanMove() const
{
    return isSurrendered_|| bCanMagicMove_;
}

// -----------------------------------------------------------------------------
// Name: DefaultMagicMoveComputer::CanMoveOverride
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
bool DefaultMagicMoveComputer::CanMoveOverride() const
{
    return isSurrendered_;
}
