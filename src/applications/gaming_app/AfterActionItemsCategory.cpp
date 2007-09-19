// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionItemsCategory.h"
#include "gaming/AfterActionModel.h"
#include "gaming/AfterActionItem.h"

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItemsCategory::AfterActionItemsCategory( QWidget* parent, const AfterActionModel& model, const std::string& category )
    : QIconView( parent )
{
    setGridX( 60 );
    setGridY( 60 );
    setSorting( true );
    setItemsMovable( false );
    setHScrollBarMode( QScrollView::AlwaysOff );

    kernel::Iterator< const AfterActionItem& > items = model.CreateIterator();
    while( items.HasMoreElements() )
    {
        const AfterActionItem& item = items.NextElement();
        if( item.GetType() == category )
            AddItem( item );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItemsCategory::~AfterActionItemsCategory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory::AddItem
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionItemsCategory::AddItem( const AfterActionItem& item )
{
    QIconViewItem* pItem = new QIconViewItem( this, item.GetName().c_str() );
    items_[ pItem->index() ] = &item;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory::dragObject
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
QDragObject* AfterActionItemsCategory::dragObject()
{
    const AfterActionItem* item = currentItem() ? items_[ currentItem()->index() ] : 0;
    if( item )
    {
        QByteArray* pBytes = new QByteArray();
        pBytes->setRawData( (const char*)&item, sizeof( AfterActionItem* ) );
        QStoredDrag* drag = new QStoredDrag( "AfterActionItem", this );
        drag->setEncodedData( *pBytes );
        return drag;
    }
    return 0;
}
