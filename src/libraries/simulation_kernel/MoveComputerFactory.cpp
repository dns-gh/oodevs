// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/MoveComputerFactory.h"
#include "simulation_kernel/DefaultMoveComputer.h"
#include "simulation_kernel/DefaultSpeedComputer.h"
#include "simulation_kernel/DefaultMaxSlopeComputer.h"

namespace moving
{
// -----------------------------------------------------------------------------
// Name: MoveComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MoveComputerFactory::MoveComputerFactory() :
		pMoveComputer_( new DefaultMoveComputer() ),
		pSpeedComputer_( new DefaultSpeedComputer()),
		pSlopeComputer_ (new DefaultMaxSlopeComputer())
{
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MoveComputerFactory::~MoveComputerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory CreateMoveComputer
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MoveComputer_ABC& MoveComputerFactory::CreateMoveComputer() const
{
    pMoveComputer_->Reset();
    return *pMoveComputer_;
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateSpeedComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
SpeedComputer_ABC& MoveComputerFactory::CreateSpeedComputer(const SpeedStrategy_ABC& strategy) const
{
	pSpeedComputer_->Reset(&strategy);
	return *pSpeedComputer_;
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateMaxSlopeComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
MaxSlopeComputer_ABC& MoveComputerFactory::CreateMaxSlopeComputer() const
{
	pSlopeComputer_->Reset();
	return *pSlopeComputer_;
}
} // namespace moving
