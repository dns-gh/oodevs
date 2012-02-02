// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichItemFactory.h"
#include "moc_RichItemFactory.cpp"
#include "ValuedListItem.h"
#include "RichLabel.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichItemFactory constructor
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
RichItemFactory::RichItemFactory( QObject* parent )
    : QObject( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory destructor
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
RichItemFactory::~RichItemFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateItem
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
ValuedListItem* RichItemFactory::CreateItem( Q3ListView * parent, ValuedListItem::Comparator comparator /*= 0*/ )
{
    return Connect( new ValuedListItem( parent, comparator ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateItem
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
ValuedListItem* RichItemFactory::CreateItem( Q3ListViewItem * parent, ValuedListItem::Comparator comparator /*= 0*/ )
{
    return Connect( new ValuedListItem( parent, comparator ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateItem
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
ValuedListItem* RichItemFactory::CreateItem( Q3ListViewItem * parent, Q3ListViewItem * after )
{
    return Connect( new ValuedListItem( parent, after ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateItem
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
ValuedListItem* RichItemFactory::CreateItem( Q3ListView* parent, Q3ListViewItem * after )
{
    return Connect( new ValuedListItem( parent, after ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::Connect
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
ValuedListItem* RichItemFactory::Connect( ValuedListItem* item )
{
    if( connections_.insert( item->listView() ).second )
        connect( item->listView(), SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnClicked( Q3ListViewItem*, const QPoint&, int ) ) );
    return item;
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::Connect
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::Connect( RichLabel* item )
{
    connect( item, SIGNAL( LinkClicked( const QString& ) ), this, SLOT( DealWithLink( const QString& ) ) );
    return item;
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::OnClicked
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void RichItemFactory::OnClicked( Q3ListViewItem* i, const QPoint& point, int column )
{
    ValuedListItem* item = ( i && i->rtti() >= 999 ) ? (ValuedListItem*)( i ) : 0;
    if( item )
    {
        const QString anchor = item->GetAnchorAt( point, column );
        if( ! anchor.isEmpty() )
            DealWithLink( anchor );
    }
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::DealWithLink
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void RichItemFactory::DealWithLink( const QString& anchor )
{
    emit LinkClicked( anchor );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( QWidget* parent /* = 0*/, const char* name /* = 0*/ )
{
    return Connect( new RichLabel( parent, name ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( const QString& text, QWidget* parent /* = 0*/, const char* name /* = 0*/ )
{
    return Connect( new RichLabel( text, parent, name ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( const QString& text, bool required, QWidget* parent /* = 0*/, const char* name /* = 0*/ )
{
    return Connect( new RichLabel( text, required, parent, name ) );
}
