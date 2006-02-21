// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Formatter.h"

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
inline
void Displayer_ABC::Display( const T& element )
{
    StartDisplay();
    AddToDisplay( element );
    EndDisplay();
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Call
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
inline
void Displayer_ABC::Call( const T& element )
{
    Formatter< T >()( element, *this );
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::AddToDisplay
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
inline
void Displayer_ABC::AddToDisplay( const T& element )
{
    VirtualTemplate< Displayer_ABC >::Call( element );
}


// -----------------------------------------------------------------------------
// Name: Displayer_ABC::AddToDisplay
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
inline
void Displayer_ABC::AddToDisplay( const QString& element )
{
    DisplayFormatted( element );
}