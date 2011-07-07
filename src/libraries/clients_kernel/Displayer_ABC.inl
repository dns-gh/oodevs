// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Formatter.h"

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Display( const T& element )
{
    StartDisplay();
    AddToDisplay( element );
    EndDisplay();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Display( const QString& name, const T& element )
{
    SubItem( name ).Display( element );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Start
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Start( const T& element )
{
    StartDisplay();
    AddToDisplay( element );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::Add
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
template< typename T >
inline
Displayer_ABC& Displayer_ABC::Add( const T& element )
{
    AddToDisplay( element );
    return *this;
};

// -----------------------------------------------------------------------------
// Name: Displayer_ABC::End
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
inline
Displayer_ABC& Displayer_ABC::End()
{
    EndDisplay();
    return *this;
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
    tools::VirtualTemplate< Displayer_ABC >::Call( element );
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

}