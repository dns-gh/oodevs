// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
#include <stdexcept>

// -----------------------------------------------------------------------------
// Name: DisplayGroup constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup::DisplayGroup( QWidget* parent, const char* name  )
    : QGroupBox( 2, Qt::Horizontal, tr( name ), parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DisplayGroup destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup::~DisplayGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::AddItem
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayGroup& DisplayGroup::AddItem( const char* name, bool bold )
{
    DisplayItem*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new DisplayItem( this, name, bold );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& DisplayGroup::SubItem( const char* name )
{
    DisplayItem* item = items_[ std::string( name ) ];
    if( ! item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' does not exist" );
    return *item;
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayGroup::StartDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayGroup::DisplayFormatted( const QString& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayGroup::EndDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayGroup::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void DisplayGroup::Clear()
{
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
        it->second->Display( "" );
}
