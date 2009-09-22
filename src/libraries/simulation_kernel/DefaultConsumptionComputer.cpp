// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultConsumptionComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"

using namespace dotation;

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionComputer::DefaultConsumptionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultConsumptionComputer::DefaultConsumptionComputer()
: pConsumptionType_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionComputer::~DefaultConsumptionComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultConsumptionComputer::~DefaultConsumptionComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionComputer::Reset
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultConsumptionComputer::Reset()
{
    pConsumptionType_ = &PHY_ConsumptionType::engineStopped_;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionComputer::SetConsumptionMode
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
bool DefaultConsumptionComputer::SetConsumptionMode( const PHY_ConsumptionType& mode )
{
    pConsumptionType_ = &mode;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionComputer::SetHeight
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& DefaultConsumptionComputer::Result() const
{
    assert( pConsumptionType_ );
    return *pConsumptionType_;
}