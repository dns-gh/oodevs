// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesTable.h"
#include "moc_PropertiesTable.cpp"
#include "TableItemDisplayer.h"
#include "PropertyTableItem.h"
#include "EditorFactory.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropertiesTable constructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesTable::PropertiesTable( QWidget* parent )
    : QTable( parent )
    , factory_( *new EditorFactory() )
    , itemDisplayer_( *new TableItemDisplayer() )
    , row_( 0 )
{
    setSelectionMode( QTable::SingleRow );
    setNumCols( 2 );
    setColumnReadOnly( 0, true );
    verticalHeader()->hide();
    setLeftMargin( 0 );
    horizontalHeader()->hide();
    setTopMargin( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setHScrollBarMode( QScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
    setFrameStyle( MenuBarPanel );
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesTable destructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesTable::~PropertiesTable()
{
    delete &itemDisplayer_;
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Hide
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::Hide()
{
    hide(); // $$$$ SBO 2006-10-18: 
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Show
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesTable::Show( bool b )
{
    setShown( b && numRows() > 0 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::SubItem
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
Displayer_ABC& PropertiesTable::SubItem( const QString& name )
{
    show();
    CIT_PropertyRows it = rows_.find( name );
    row_ = 0;
    if( it == rows_.end() )
    {
        row_ = numRows();
        insertRows( row_ );
        rows_[name] = row_;
        setText( row_, 0, name );
    }
    else
        row_ = it->second;
    return *this;
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesTable::StartDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::StartDisplay()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesTable::DisplayFormatted
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::DisplayFormatted( const QString& )
{
    NotToBeCalled( __FUNCTION__ );
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesTable::EndDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::EndDisplay()
{
    clearSelection();
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::Call( kernel::Property_ABC* const& property )
{
    setItem( row_, 1, new PropertyTableItem( this, *property, itemDisplayer_, factory_ ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::focusOutEvent
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void PropertiesTable::focusOutEvent( QFocusEvent* event )
{
    clearSelection();
    QTable::focusOutEvent( event );
}
