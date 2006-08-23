// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ListItemDisplayer.h"
#include "RichListItem.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
ListItemDisplayer::ListItemDisplayer()
    : item_( 0 )
    , column_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer destructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
ListItemDisplayer::~ListItemDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::AddColumn
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
ListItemDisplayer& ListItemDisplayer::AddColumn( const QString& column )
{
    columns_.push_back( column );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::operator()
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& ListItemDisplayer::operator()( RichListItem* item )
{
    column_ = 0;
    item_ = item;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::Call
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
void ListItemDisplayer::Call( const QColor& color )
{
    if( ! item_ )
        throw std::runtime_error( "ListItemDisplayer : Item not set" );
    item_->SetFontColor( color );
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& ListItemDisplayer::SubItem( const QString& name )
{
    if( ! name.isNull() )
        column_ = FindColumn( name );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void ListItemDisplayer::StartDisplay()
{
    message_ = "";
}
    
// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void ListItemDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void ListItemDisplayer::EndDisplay()
{
    if( ! item_ )
        throw std::runtime_error( "ListItemDisplayer : Item not set" );
    if( column_ < 0 )
        throw std::runtime_error( "ListItemDisplayer : Colunm not set" );
    item_->setText( column_, message_ );
    column_ = ( column_ + 1 ) % columns_.size();
}
    
// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::FindColumn
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
int ListItemDisplayer::FindColumn( const QString& name ) const
{
    for( unsigned i = 0; i < columns_.size(); ++i )
        if( columns_[i] == name )
            return i;
    throw std::runtime_error( std::string( "Column '" ) + name.ascii() + "' does not exist" );
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ListItemDisplayer::Hide()
{
    // NOTHING
}
