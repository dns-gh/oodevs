// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Displayer_ABC.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Displayer_ABC constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Displayer_ABC::Displayer_ABC()
    : tools::VirtualTemplate< Displayer_ABC > ( this )
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
    throw MASA_EXCEPTION( std::string() + "Function '" + function + "' of '" + typeid(*this).name() + "' should not be called" );
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::IsValid
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
bool Displayer_ABC::IsValid() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::End
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& Displayer_ABC::End()
{
    EndDisplay();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::AddToDisplay
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Displayer_ABC::AddToDisplay( const QString& element )
{
    DisplayFormatted( element );
}