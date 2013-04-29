// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ValuedListItem.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( Q3ListView * parent, Comparator comparator /*= 0*/ )
    : RichListItem( parent )
    , container_ ( 0 )
    , comparator_( comparator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( Q3ListViewItem * parent, Comparator comparator /*= 0*/ )
    : RichListItem( parent )
    , container_ ( 0 )
    , comparator_( comparator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( Q3ListViewItem * parent, Q3ListViewItem* after, Comparator comparator /*= 0*/ )
    : RichListItem( parent, after )
    , container_ ( 0 )
    , comparator_( comparator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( Q3ListView* parent, Q3ListViewItem* after, Comparator comparator /*= 0*/ )
    : RichListItem( parent, after )
    , container_ ( 0 )
    , comparator_( comparator )
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
// Name: ValuedListItem::rtti
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
int ValuedListItem::rtti() const
{
    return 1000;
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::compare
// Created: JSR 2012-02-01
// -----------------------------------------------------------------------------
int ValuedListItem::compare( Q3ListViewItem *i, int col, bool ascending ) const
{
    if( comparator_ )
    {
        ValuedListItem* item = dynamic_cast< ValuedListItem* >( i );
        if( item )
            return comparator_( *this, *item, col, ascending );
    }
    return RichListItem::compare( i, col, ascending );
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

// -----------------------------------------------------------------------------
// Name: ValueContainer::Select
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void ValueContainer< const Entity_ABC >::Select( kernel::ActionController& actions )
{
    if( value_ )
        value_->Select( actions );
}

// -----------------------------------------------------------------------------
// Name: ValueContainer::ContextMenu
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void ValueContainer< const Entity_ABC >::ContextMenu( kernel::ActionController& actions, const QPoint& where )
{
    if( value_ )
        value_->ContextMenu( actions, where );
}

// -----------------------------------------------------------------------------
// Name: ValueContainer::Activate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void ValueContainer< const Entity_ABC >::Activate( kernel::ActionController& actions )
{
    if( value_ )
        value_->Activate( actions );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::SetToolTip
// Created: SBO 2007-01-05
// -----------------------------------------------------------------------------
void ValuedListItem::SetToolTip( const QString& text )
{
    toolTip_ = text;
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::GetToolTip
// Created: SBO 2007-01-08
// -----------------------------------------------------------------------------
QString ValuedListItem::GetToolTip() const
{
    return toolTip_;
}