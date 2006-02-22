// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Displayer.h"
#include "DisplayGroup.h"

// -----------------------------------------------------------------------------
// Name: Displayer constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
Displayer::Displayer( QWidget* parent )
    : parent_( parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Displayer destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
Displayer::~Displayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Displayer::AddGroup
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup& Displayer::AddGroup( const char* name )
{
    DisplayGroup*& group = groups_[ std::string( name ) ];
    if( group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' already exists" );
    group = new DisplayGroup( parent_, name );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: Displayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& Displayer::SubItem( const char* name )
{
    DisplayGroup* group = groups_[ std::string( name ) ];
    if( ! group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' does not exist" );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: Displayer::Group
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
DisplayGroup& Displayer::Group( const char* name )
{
    return (DisplayGroup&)( SubItem( name ) );
}

// -----------------------------------------------------------------------------
// Name: Displayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Displayer::StartDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Displayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Displayer::DisplayFormatted( const QString& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Displayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Displayer::EndDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Displayer::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void Displayer::Clear()
{
    for( IT_Groups it = groups_.begin(); it != groups_.end(); ++it )
        it->second->Clear();
}