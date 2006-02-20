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
// Name: Displayer::Group
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup& Displayer::Group( const char* name )
{
    DisplayGroup* group = groups_[ std::string( name ) ];
    if( ! group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' does not exist" );
    return *group;
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

// -----------------------------------------------------------------------------
// Name: Displayer::YesNo
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString Displayer::YesNo( bool value )
{
    return value ? qApp->tr( "Oui" ) : qApp->tr( "Non" );
}

// -----------------------------------------------------------------------------
// Name: Displayer::Id
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString Displayer::Id( unsigned int value )
{
    return value ? QString::number( value ) : "-";
}
