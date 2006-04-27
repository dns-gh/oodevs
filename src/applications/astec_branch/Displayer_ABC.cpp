// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: Displayer_ABC constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Displayer_ABC::Displayer_ABC()
    : VirtualTemplate< Displayer_ABC > ( this )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Displayer_ABC::~Displayer_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::NotToBeCalled
// Created: AGE 2006-04-27
// -----------------------------------------------------------------------------
Displayer_ABC& Displayer_ABC::NotToBeCalled( const char* function ) const
{
    throw std::runtime_error( std::string() + "Function '" + function + "' of '" + typeid(*this).name() + "' should not be called" );
}
