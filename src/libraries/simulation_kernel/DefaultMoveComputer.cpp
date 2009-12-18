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
: bCanMove_( true ), isSurrendered_(false)
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
// Name: DefaultMoveComputer::NotifyReinforcing
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultMoveComputer::NotifyReinforcing()
{
	bCanMove_ = false;
}
// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::NotifyTransported
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMoveComputer::NotifyTransported()
{
	bCanMove_ = false;
}
// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::NotifySurrendered
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMoveComputer::NotifySurrendered()
{
	isSurrendered_ = true;
}
// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::NotifyCannotFly
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void DefaultMoveComputer::NotifyCannotFly()
{
	bCanMove_ = false;
}
// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::CanMove
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
bool DefaultMoveComputer::CanMove() const
{
    return isSurrendered_|| bCanMove_;
}
// -----------------------------------------------------------------------------
// Name: DefaultMoveComputer::CanMoveOverride
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
bool DefaultMoveComputer::CanMoveOverride() const
{
	return isSurrendered_;
}
