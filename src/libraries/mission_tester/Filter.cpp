// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Filter.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Filter constructor
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
Filter::Filter( const std::string& criterion )
    : criterion_( criterion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter destructor
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
Filter::~Filter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter::Accepts
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Filter::Accepts( const std::string& criterion ) const
{
    return criterion_ == criterion;
}
