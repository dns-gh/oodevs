// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Display.h"
#include "MOS_DisplayGroup.h"

// -----------------------------------------------------------------------------
// Name: MOS_Display constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_Display::MOS_Display( QWidget* parent )
    : parent_( parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Display destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_Display::~MOS_Display()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Display::AddGroup
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayGroup& MOS_Display::AddGroup( const char* name )
{
    MOS_DisplayGroup*& group = groups_[ std::string( name ) ];
    if( group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' already exists" );
    group = new MOS_DisplayGroup( parent_, name );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: MOS_Display::Group
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayGroup& MOS_Display::Group( const char* name )
{
    MOS_DisplayGroup* group = groups_[ std::string( name ) ];
    if( ! group )
        throw std::runtime_error( "Group '" + std::string( name ) + "' does not exist" );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: MOS_Display::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void MOS_Display::Clear()
{
    for( IT_Groups it = groups_.begin(); it != groups_.end(); ++it )
        it->second->Clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_Display::YesNo
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString MOS_Display::YesNo( bool value )
{
    return value ? qApp->tr( "Oui" ) : qApp->tr( "Non" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Display::Id
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
QString MOS_Display::Id( unsigned int value )
{
    return value ? QString::number( value ) : "-";
}
