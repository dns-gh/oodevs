// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Symbol.h"

// -----------------------------------------------------------------------------
// Name: Symbol constructor
// Created: JSR 2011-08-04
// -----------------------------------------------------------------------------
Symbol::Symbol( const std::string& symbol )
{
    if( !symbol.empty() )
        OverrideValue( symbol );
}

// -----------------------------------------------------------------------------
// Name: Symbol destructor
// Created: JSR 2011-08-04
// -----------------------------------------------------------------------------
Symbol::~Symbol()
{
    // NOTHING
}
