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
#include "gaming/AfterActionFactory.h"

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItemsCategory::AfterActionItemsCategory( QWidget* parent, const AfterActionModel& model, const QString& category )
    : QIconView( parent )
{
    setGridX( 60 );
    setGridY( 60 );
    setSorting( true );
    setItemsMovable( false );
    setHScrollBarMode( QScrollView::AlwaysOff );

    kernel::Iterator< const AfterActionFactory& > items = model.CreateIterator();
    while( items.HasMoreElements() )
    {
        const AfterActionFactory& item = items.NextElement();
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
void AfterActionItemsCategory::AddItem( const AfterActionFactory& item )
{
    QIconViewItem* pItem = new QIconViewItem( this, item.GetName() );
    items_[ pItem->index() ] = &item;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItemsCategory::dragObject
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
QDragObject* AfterActionItemsCategory::dragObject()
{
    const AfterActionFactory* item = currentItem() ? items_[ currentItem()->index() ] : 0;
    if( item )
    {
        QByteArray* pBytes = new QByteArray();
        pBytes->setRawData( (const char*)&item, sizeof( AfterActionFactory* ) );
        QStoredDrag* drag = new QStoredDrag( "AfterActionFactory", this );
        drag->setEncodedData( *pBytes );
        return drag;
    }
    return 0;
}
