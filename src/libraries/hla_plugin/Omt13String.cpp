// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Omt13String.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Omt13String constructor
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
Omt13String::Omt13String()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Omt13String constructor
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
Omt13String::Omt13String( const std::string& data )
    : data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Omt13String destructor
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
Omt13String::~Omt13String()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Omt13String::str
// Created: SLI 2011-09-22
// -----------------------------------------------------------------------------
std::string Omt13String::str() const
{
    return data_;
}

// -----------------------------------------------------------------------------
// Name: Omt13StringArray constructor
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
Omt13StringArray::Omt13StringArray()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Omt13StringArray destructor
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
Omt13StringArray::~Omt13StringArray()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Omt13StringArray::Add
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void Omt13StringArray::Add( const std::string& name )
{
    values_.push_back( Omt13String( name ) );
}
