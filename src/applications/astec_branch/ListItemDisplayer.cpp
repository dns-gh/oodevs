// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ListItemDisplayer.h"

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
ListItemDisplayer::ListItemDisplayer()
    : item_( 0 )
    , column_( -1 )
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
ListItemDisplayer& ListItemDisplayer::AddColumn( const char* column )
{
    columns_.push_back( column );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::operator()
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& ListItemDisplayer::operator()( QListViewItem* item )
{
    item_ = item;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& ListItemDisplayer::SubItem( const char* name )
{
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
}
    
// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::FindColumn
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
int ListItemDisplayer::FindColumn( const char* name ) const
{
    for( int i = 0; i < columns_.size(); ++i )
        if( columns_[i] == name )
            return i;
    throw std::runtime_error( std::string( "Column '" ) + name + "' does not exist" );
}

// -----------------------------------------------------------------------------
// Name: ListItemDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ListItemDisplayer::Hide()
{

}
