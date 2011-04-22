// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Timeout.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Timeout constructor
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
Timeout::Timeout( unsigned int duration )
    : duration_( duration )
    , start_   ( boost::posix_time::microsec_clock::universal_time() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Timeout destructor
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
Timeout::~Timeout()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Timeout::Expired
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
bool Timeout::Expired() const
{
    return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
}
