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
// Created: FPT 2011-03-25
// -----------------------------------------------------------------------------
ListItemToolTip::ListItemToolTip( Q3ListView& list )
    : listView_( list )
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
    if( listView_.showToolTips() )
        if( ValuedListItem* item = static_cast< ValuedListItem* >( listView_.itemAt( pos ) ) )
            listView_.setToolTip( item->GetToolTip() );
}
