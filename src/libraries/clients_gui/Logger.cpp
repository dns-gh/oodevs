// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::Logger */

#include "clients_gui_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"
#include <qdatetime.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent, ItemFactory_ABC& factory )
    : QListView( pParent )
    , factory_( factory )
{
    setMinimumSize( 1, 1 );
    setShowSortIndicator( true );
    setSorting( -1 );
    setRootIsDecorated( true );
    addColumn( tr( "Date" ) );
    addColumn( tr( "Message" ) );
    setResizeMode( QListView::LastColumn );
    setAllColumnsShowFocus ( true );

    popupMenu_.insertItem( tr( "Clear list" ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger::Info
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Info()
{
    return StartLog( Qt::black, false );
}

// -----------------------------------------------------------------------------
// Name: Logger::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Warning()
{
    return StartLog( Qt::darkRed, true );
}

// -----------------------------------------------------------------------------
// Name: Logger::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Error()
{
    return StartLog( Qt::red, true );
}

// -----------------------------------------------------------------------------
// Name: Logger::StartLog
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::StartLog( const QColor& color, bool popup )
{
    ValuedListItem* pItem = factory_.CreateItem( this );
    pItem->setText( 0, QTime::currentTime().toString() );
    pItem->SetFontColor( color );
    std::stringstream* output = new std::stringstream();
    items_[ output ] = T_Item( pItem, popup );
    return LogElement( *this, *output );
}

// -----------------------------------------------------------------------------
// Name: Logger::End
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void Logger::End( std::stringstream& output )
{
    T_Item& item = items_[ &output ];
    if( item.first )
        item.first->setText( 1, output.str().c_str() );
    if( item.second )
        emit EmitError();
    items_.erase( &output );
    delete &output;
}

// -----------------------------------------------------------------------------
// Name: Logger::OnRequestPopup
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::OnRequestPopup( QListViewItem* /*pItem*/, const QPoint& pos )
{
    popupMenu_.popup( pos );
}
