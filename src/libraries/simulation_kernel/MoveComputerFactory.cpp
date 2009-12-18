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
MoveComputerFactory::MoveComputerFactory()
{
    // NOTHING
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
std::auto_ptr< MoveComputer_ABC > MoveComputerFactory::CreateMoveComputer() const
{
    std::auto_ptr< MoveComputer_ABC > pMoveComputer( new DefaultMoveComputer() );
    return pMoveComputer;
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateSpeedComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
std::auto_ptr< SpeedComputer_ABC > MoveComputerFactory::CreateSpeedComputer( const SpeedStrategy_ABC& strategy ) const
{
	std::auto_ptr< SpeedComputer_ABC > pSpeedComputer( new DefaultSpeedComputer( strategy ) );
	return pSpeedComputer;
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateMaxSlopeComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
std::auto_ptr< MaxSlopeComputer_ABC > MoveComputerFactory::CreateMaxSlopeComputer() const
{
	std::auto_ptr< MaxSlopeComputer_ABC > pSlopeComputer( new DefaultMaxSlopeComputer() );
	return pSlopeComputer;
}
} // namespace moving
