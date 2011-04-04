// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AccommodationType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AccommodationType constructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
AccommodationType::AccommodationType( const std::string& role, double /*maxCapacity*/, double /*nominalCapacity*/ )
    : role_( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AccommodationType destructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
AccommodationType::~AccommodationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AccommodationType::GetRole
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
const std::string& AccommodationType::GetRole() const
{
    return role_;
}
