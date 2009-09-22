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

using namespace moving;
// -----------------------------------------------------------------------------
// Name: MoveComputerFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MoveComputerFactory::MoveComputerFactory() 
: pMoveComputer_( new DefaultMoveComputer() )
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
// Name: MoveComputerFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MoveComputer_ABC& MoveComputerFactory::Create() const
{
    pMoveComputer_->Reset();
    return *pMoveComputer_;
}