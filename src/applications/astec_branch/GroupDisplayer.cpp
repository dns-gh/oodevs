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
#include "SpinBoxDisplayer.h"
#include "CheckBoxDisplayer.h"
#include "TextFieldDisplayer.h"
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
    Displayer_ABC*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new LabelDisplayer( this, name, bold );
    return *this;
}

// $$$$ AGE 2006-02-23: factor
// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddSpinBox
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddSpinBox( const char* name, int min, int max, int step )
{
    Displayer_ABC*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new SpinBoxDisplayer( this, name, min, max, step );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddCheckBox
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddCheckBox( const char* name )
{
    Displayer_ABC*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new CheckBoxDisplayer( this, name );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddTextField
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddTextField( const char* name )
{
    Displayer_ABC*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new TextFieldDisplayer( this, name );
    return *this;
}
    
// -----------------------------------------------------------------------------
// Name: GroupDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& GroupDisplayer::SubItem( const char* name )
{
    Displayer_ABC* item = items_[ std::string( name ) ];
    if( ! item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' does not exist" );
    show();
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
// Name: GroupDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void GroupDisplayer::Hide()
{
    hide();
}
