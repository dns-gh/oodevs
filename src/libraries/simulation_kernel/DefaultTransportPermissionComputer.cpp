// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultTransportPermissionComputer.h"

namespace transport
{

// -----------------------------------------------------------------------------
// Name: DefaultTransportPermissionComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportPermissionComputer::DefaultTransportPermissionComputer() :
    allow_( true )
{
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportPermissionComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultTransportPermissionComputer::~DefaultTransportPermissionComputer()
{
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportPermissionComputer::Reset
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportPermissionComputer::Reset()
{
    allow_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportPermissionComputerAllowLoading
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultTransportPermissionComputer::AllowLoading( bool doAllow )
{
    allow_ &= doAllow;
}

// -----------------------------------------------------------------------------
// Name: DefaultTransportPermissionComputer::CanBeLoaded
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
bool DefaultTransportPermissionComputer::CanBeLoaded() const
{
    return allow_;
}

}
