// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateMarking.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateMarking constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
AggregateMarking::AggregateMarking( const std::string& name )
    : markingType_( 1 ) // ASCII
{
    ::memset( markingData_, 0, sizeof( markingData_ ) );
    const unsigned length = std::min( sizeof( markingData_ ), name.length() );
    ::memcpy( markingData_, name.c_str(), length );
}

// -----------------------------------------------------------------------------
// Name: AggregateMarking destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
AggregateMarking::~AggregateMarking()
{
    // NOTHING
}
