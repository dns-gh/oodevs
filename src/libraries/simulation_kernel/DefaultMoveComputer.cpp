// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultMoveComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"

using namespace moving;

// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::DefaultMoveComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultMoveComputer::DefaultMoveComputer()
: bCanMove_( true )
{

}

// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::~DefaultMoveComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultMoveComputer::~DefaultMoveComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::Reset
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultMoveComputer::Reset()
{
    bCanMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::NotifyNoDotation
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultMoveComputer::NotifyNoDotation()
{
    bCanMove_ = false;
    //@TODO MGD ADD error message to know why we can't move?
}

// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::CanMove
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
bool DefaultMoveComputer::CanMove() const
{
    return bCanMove_;
}