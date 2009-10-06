// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultLoadedStateConsistencyComputer.h"

namespace transport
{

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultLoadedStateConsistencyComputer::DefaultLoadedStateConsistencyComputer() :
    hasValidCarrier_(false),
    hasValidLoadable_(false)
{
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultLoadedStateConsistencyComputer::~DefaultLoadedStateConsistencyComputer()
{
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer::Reset
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultLoadedStateConsistencyComputer::Reset()
{
    hasValidCarrier_ = hasValidLoadable_ = false;
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer::EnableCarrier
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultLoadedStateConsistencyComputer::EnableCarrier(bool value)
{
    hasValidCarrier_ |= value;
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer::EnableLoadable
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultLoadedStateConsistencyComputer::EnableLoadable(bool value)
{
    hasValidLoadable_ |= value;
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer::HasValidCarrier
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
bool DefaultLoadedStateConsistencyComputer::HasValidCarrier() const
{
    return hasValidCarrier_;
}

// -----------------------------------------------------------------------------
// Name: DefaultLoadedStateConsistencyComputer::HasValidLoadable
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
bool DefaultLoadedStateConsistencyComputer::HasValidLoadable() const
{
    return hasValidLoadable_;
}
}
