// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListView * parent )
    : RichListItem( parent )
    , container_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListViewItem * parent )
    : RichListItem( parent )
    , container_( 0 )
{
    // NOTHING
}
   
// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< >
ValuedListItem::ValuedListItem( QListViewItem * const & parent, QListViewItem * after )
    : RichListItem( parent, after )
    , container_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< >
ValuedListItem::ValuedListItem( QListView* const & parent, QListViewItem * after )
    : RichListItem( parent, after )
    , container_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ValuedListItem::~ValuedListItem()
{
    delete container_;
}

// -----------------------------------------------------------------------------
// Name: EmptyListItem::EmptyListItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
EmptyListItem::EmptyListItem( QListView * parent )
    : ValuedListItem( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EmptyListItem::EmptyListItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
EmptyListItem::EmptyListItem( QListViewItem * parent )
    : ValuedListItem( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EmptyListItem::EmptyListItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
EmptyListItem::EmptyListItem( QListView* value, QListViewItem * parent )
    : ValuedListItem( value, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EmptyListItem::EmptyListItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
EmptyListItem::EmptyListItem( QListViewItem* parent, QListViewItem * after )
    : ValuedListItem( parent, after )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::rtti
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
int ValuedListItem::rtti() const
{
    return container_ ? container_->rtti() : 1000;
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::Select
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ValuedListItem::Select( ActionController& actions )
{
    if( container_ )
        container_->Select( actions );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::ContextMenu
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ValuedListItem::ContextMenu( ActionController& actions, const QPoint& where )
{
    if( container_ )
        container_->ContextMenu( actions, where );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::Activate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ValuedListItem::Activate( ActionController& actions )
{
    if( container_ )
        container_->Activate( actions );
}
