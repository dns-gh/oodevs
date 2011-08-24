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
ListItemToolTip::ListItemToolTip( QWidget* parent, Q3ListView& list )
    : QObject( parent )
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
// Name: ListItemToolTip::eventFilter
// Created: RBA 2011-08-24
// -----------------------------------------------------------------------------
bool ListItemToolTip::eventFilter( QObject *obj, QEvent *event )
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        QPoint pos = helpEvent->pos();
        if( !parent() || !listView_.showToolTips() )
            return false;
        ValuedListItem* item = static_cast< ValuedListItem* >( listView_.itemAt( pos ) );
        if( !item )
            return false;
        QRect rect = listView_.itemRect( item );
        QToolTip::showText(helpEvent->globalPos(), item->GetToolTip(), (QWidget*) parent(), rect );
        return true;
    }
    return false;
}
