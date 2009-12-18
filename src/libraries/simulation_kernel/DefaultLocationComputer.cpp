// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultLocationComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"

using namespace location;

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::DefaultLocationComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultLocationComputer::DefaultLocationComputer()
: height_( 0 )
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
void DefaultLocationComputer::SetHeight( double height )
{
    height_ = height;
}

// -----------------------------------------------------------------------------
// Name: DefaultLocationComputer::SetHeight
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
double DefaultLocationComputer::GetHeight() const
{
    return height_;
}