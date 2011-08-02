// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultLocationComputer.h"

using namespace location;

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::DefaultLocationComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultLocationComputer::DefaultLocationComputer()
    : height_( 0 )
    , flying_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::~DefaultLocationComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultLocationComputer::~DefaultLocationComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::SetHeight
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultLocationComputer::IncreaseHeight( double height )
{
    height_ += height;
}

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::SetHeight
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
double DefaultLocationComputer::GetHeight() const
{
    return flying_ ? height_ : 0;
}

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::SetFlying
// Created: LDC 2011-08-02
// -----------------------------------------------------------------------------
void DefaultLocationComputer::SetFlying()
{
    flying_ = true;
}
