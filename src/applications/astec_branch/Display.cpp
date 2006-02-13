// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Display.h"
#include "DisplayGroup.h"

// -----------------------------------------------------------------------------
// Name: Display constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
Display::Display( QWidget* parent )
    : parent_( parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Display destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
Display::~Display()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Display::AddGroup
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup& Display::AddGroup( const char* name )
{
    DisplayGroup*& group = groups_[ std::string( name ) ];
    if( group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' already exists" );
    group = new DisplayGroup( parent_, name );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: Display::Group
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup& Display::Group( const char* name )
{
    DisplayGroup* group = groups_[ std::string( name ) ];
    if( ! group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' does not exist" );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: Display::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void Display::Clear()
{
    for( IT_Groups it = groups_.begin(); it != groups_.end(); ++it )
        it->second->Clear();
}

// -----------------------------------------------------------------------------
// Name: Display::YesNo
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString Display::YesNo( bool value )
{
    return value ? qApp->tr( "Oui" ) : qApp->tr( "Non" );
}

// -----------------------------------------------------------------------------
// Name: Display::Id
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString Display::Id( unsigned int value )
{
    return value ? QString::number( value ) : "-";
}
