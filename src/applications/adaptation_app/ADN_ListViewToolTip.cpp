// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListViewToolTip.h"
#include "ADN_ListViewItem.h"
#include "ADN_ListView.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListViewToolTip constructor
// Created: RBA 2011-08-24
// -----------------------------------------------------------------------------
ADN_ListViewToolTip::ADN_ListViewToolTip( QWidget* parent, ADN_ListView& list )
    : QObject( parent )
    , listView_( list )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListViewToolTip destructor
// Created: RBA 2011-08-24
// -----------------------------------------------------------------------------
ADN_ListViewToolTip::~ADN_ListViewToolTip()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListViewToolTip::eventFilter
// Created: RBA 2011-08-24
// -----------------------------------------------------------------------------
bool ADN_ListViewToolTip::eventFilter( QObject*, QEvent* event )
{
     if( event->type() == QEvent::ToolTip )
     {
         QHelpEvent* helpEvent = static_cast< QHelpEvent* >( event );
         QPoint pos = helpEvent->pos();
         if( !parent() /*|| !listView_.showToolTips()*/ )
             return false;
         QModelIndex index = listView_.indexAt( pos );
         if( !index.isValid() )
             return false;
         QString text = listView_.GetToolTipFor( index ).c_str();
         QRect rect = listView_.visualRect( index );
         QToolTip::showText( helpEvent->globalPos(), text, static_cast< QWidget* >( parent() ), rect );
         return true;
    }
    return false;
}
