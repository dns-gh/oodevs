// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NullTerminatedAsciiString.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NullTerminatedAsciiString constructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
NullTerminatedAsciiString::NullTerminatedAsciiString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullTerminatedAsciiString constructor
// Created: SLI 2011-06-23
// -----------------------------------------------------------------------------
NullTerminatedAsciiString::NullTerminatedAsciiString( const std::string& data )
    : data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullTerminatedAsciiString destructor
// Created: SLI 2011-06-23
// -----------------------------------------------------------------------------
NullTerminatedAsciiString::~NullTerminatedAsciiString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullTerminatedAsciiString::str
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
std::string NullTerminatedAsciiString::str() const
{
    return data_;
}
