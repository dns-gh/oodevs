// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SubItemDisplayer.h"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SubItemDisplayer::SubItemDisplayer( const QString& name, ItemFactory_ABC& factory )
    : name_( name )
    , factory_( factory )
    , parent_( 0 )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SubItemDisplayer::~SubItemDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::Hide
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SubItemDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::AddChild
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SubItemDisplayer& SubItemDisplayer::AddChild( const QString& child )
{
    children_.push_back( child );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::operator()
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Displayer_ABC& SubItemDisplayer::operator()( Q3ListViewItem* item )
{
    parent_ = item;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::SubItem
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Displayer_ABC& SubItemDisplayer::SubItem( const QString& name )
{
    current_ = FindAddChild( name );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::StartDisplay
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SubItemDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::DisplayFormatted
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SubItemDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::EndDisplay
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SubItemDisplayer::EndDisplay()
{
    current_->setText( 1, message_ );
}


// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::LastChild
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
Q3ListViewItem* SubItemDisplayer::LastSibling()
{
    Q3ListViewItem* previous = parent_->firstChild();
    Q3ListViewItem* child = previous;
    while( child )
    {
        previous = child;
        child = child->nextSibling();
    }
    return previous;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::{
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
Q3ListViewItem* SubItemDisplayer::FindChild( const QString& name )
{
    Q3ListViewItem* child = parent_->firstChild();
    while( child )
    {
        if ( child->text( 0 ) == name )
            return child;
        child = child->nextSibling();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: SubItemDisplayer::FindAddChild
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Q3ListViewItem* SubItemDisplayer::FindAddChild( const QString& name )
{
    if( ! parent_ )
        throw std::runtime_error( "Parent not set" );
    if( name == name_ )
        return parent_;
    if( find( children_.begin(), children_.end(), name ) == children_.end() )
        AddChild( name );
    Q3ListViewItem* foundItem = FindChild( name );
    if ( foundItem )
        return foundItem;

    Q3ListViewItem* newItem = factory_.CreateItem( parent_, LastSibling() );
    assert( newItem );
    newItem->setText( 0, name );
    return newItem;
}
