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
#include "simulation_kernel/DefaultMagicMoveComputer.h"

using namespace moving;

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
    return std::auto_ptr< MoveComputer_ABC >( new DefaultMoveComputer() );
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory CreateMoveComputer
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
std::auto_ptr< MoveComputer_ABC > MoveComputerFactory::CreateMagicMoveComputer() const
{
    return std::auto_ptr< MoveComputer_ABC >( new DefaultMagicMoveComputer() );
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateSpeedComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
std::auto_ptr< SpeedComputer_ABC > MoveComputerFactory::CreateSpeedComputer( const SpeedStrategy_ABC& strategy ) const
{
    return std::auto_ptr< SpeedComputer_ABC >( new DefaultSpeedComputer( strategy ) );
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory::CreateSpeedComputer
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
std::auto_ptr< SpeedComputer_ABC > MoveComputerFactory::CreateSpeedComputer( const SpeedStrategy_ABC& strategy, bool loaded ) const
{
    if( loaded )
        return std::auto_ptr< SpeedComputer_ABC >( new LoadedSpeedComputer( strategy ) );
    return std::auto_ptr< SpeedComputer_ABC >( new UnloadedSpeedComputer( strategy ) );
}

// -----------------------------------------------------------------------------
// Name: MoveComputerFactory MoveComputerFactory::CreateMaxSlopeComputer
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
std::auto_ptr< MaxSlopeComputer_ABC > MoveComputerFactory::CreateMaxSlopeComputer() const
{
    return std::auto_ptr< MaxSlopeComputer_ABC >( new DefaultMaxSlopeComputer() );
}
