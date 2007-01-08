// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ListItemToolTip.h"
#include "ValuedListItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ListItemToolTip constructor
// Created: SBO 2007-01-08
// -----------------------------------------------------------------------------
ListItemToolTip::ListItemToolTip( QWidget* parent, QListView& list )
    : QToolTip( parent )
    , listView_( list )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListItemToolTip destructor
// Created: SBO 2007-01-08
// -----------------------------------------------------------------------------
ListItemToolTip::~ListItemToolTip()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListItemToolTip::maybeTip
// Created: SBO 2007-01-08
// -----------------------------------------------------------------------------
void ListItemToolTip::maybeTip( const QPoint& pos )
{
    if ( !parentWidget() || !listView_.showToolTips() )
        return;
    ValuedListItem* item = static_cast< ValuedListItem* >( listView_.itemAt( pos ) );
    if ( !item )
        return;
    QRect rect = listView_.itemRect( item );
    tip( rect, item->GetToolTip() );
}
