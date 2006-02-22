// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include <stdexcept>

// -----------------------------------------------------------------------------
// Name: GroupDisplayer constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
GroupDisplayer::GroupDisplayer( QWidget* parent, const char* name  )
    : QGroupBox( 2, Qt::Horizontal, tr( name ), parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: GroupDisplayer destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
GroupDisplayer::~GroupDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddLabel
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddLabel( const char* name, bool bold )
{
    LabelDisplayer*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new LabelDisplayer( this, name, bold );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& GroupDisplayer::SubItem( const char* name )
{
    LabelDisplayer* item = items_[ std::string( name ) ];
    if( ! item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' does not exist" );
    return *item;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::StartDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::DisplayFormatted( const QString& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::EndDisplay()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void GroupDisplayer::Clear()
{
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
        it->second->Display( "" );
}
