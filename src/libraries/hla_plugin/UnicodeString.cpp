// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnicodeString.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UnicodeString constructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
UnicodeString::UnicodeString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnicodeString constructor
// Created: SLI 2011-06-23
// -----------------------------------------------------------------------------
UnicodeString::UnicodeString( const std::string& data )
    : data_( data.begin(), data.end() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnicodeString destructor
// Created: SLI 2011-06-23
// -----------------------------------------------------------------------------
UnicodeString::~UnicodeString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnicodeString::str
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
std::string UnicodeString::str() const
{
    return std::string( data_.begin(), data_.end() );
}
