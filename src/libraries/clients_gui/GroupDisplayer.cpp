// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "SpinBoxDisplayer.h"
#include "CheckBoxDisplayer.h"
#include <stdexcept>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GroupDisplayer constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
GroupDisplayer::GroupDisplayer( QWidget* parent, const QString& name, ItemFactory_ABC& factory )
    : QGroupBox( name, parent )
    , factory_( factory )
{
    layout_ = new QVBoxLayout( this );
    setLayout( layout_ );
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
GroupDisplayer& GroupDisplayer::AddLabel( const QString& name, bool bold )
{
    Displayer_ABC*& item = items_[ name ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name.ascii() ) + "' already added" );
    item = new LabelDisplayer( layout_, name, bold, factory_ );
    return *this;
}

// $$$$ AGE 2006-02-23: factor
// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddSpinBox
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddSpinBox( const QString& name, int min, int max, int step )
{
    Displayer_ABC*& item = items_[ name ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name.ascii() ) + "' already added" );
    item = new SpinBoxDisplayer( layout_, name, min, max, step );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::AddCheckBox
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
GroupDisplayer& GroupDisplayer::AddCheckBox( const QString& name )
{
    Displayer_ABC*& item = items_[ name ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name.ascii() ) + "' already added" );
    item = new CheckBoxDisplayer( layout_, name );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& GroupDisplayer::SubItem( const QString& name )
{
    Displayer_ABC* item = items_[ name ];
    if( ! item )
        throw std::runtime_error( "Item '" + std::string( name.ascii() ) + "' does not exist" );
    show();
    return *item;
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::StartDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::DisplayFormatted( const QString& )
{
    DisplayNotImplemented< QString >();
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void GroupDisplayer::EndDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void GroupDisplayer::Hide()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: GroupDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void GroupDisplayer::Clear()
{
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
        it->second->Clear();
}
