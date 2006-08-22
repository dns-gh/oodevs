// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"

#include "MT/MT_Logger/MT_logger_lib.h"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent, ItemFactory_ABC& factory )
    : QListView( pParent )
    , MT_Logger_ABC ()
    , factory_( factory )
{
    MT_LogManager::Instance().RegisterLogger( *this );

    setMinimumSize( 1, 1 );
    setShowSortIndicator( true );
    setSorting( -1 );
    setRootIsDecorated( true );
    addColumn( tr( "Recu" ) );
    addColumn( tr( "Log" ) );
    setResizeMode( QListView::LastColumn );
    setAllColumnsShowFocus ( true );

    popupMenu_.insertItem( tr( "Effacer liste" ), this, SLOT( clear() ) ); 

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}


// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    MT_LogManager::Instance().UnregisterLogger( *this );
}

namespace
{
    QColor ChooseMessageColor( MT_Logger_ABC::E_LogLevel level )
    {
        switch( level )
        {
        case MT_Logger_ABC::eLogLevel_Error:
        case MT_Logger_ABC::eLogLevel_FatalError: 
            return Qt::red;
        case MT_Logger_ABC::eLogLevel_Warning:
            return Qt::darkRed;
        case MT_Logger_ABC::eLogLevel_Message:
        case MT_Logger_ABC::eLogLevel_Info:
            return Qt::black;
        };
        return Qt::darkBlue;
    };

    bool IsErrorMesssage( MT_Logger_ABC::E_LogLevel level )
    {
        return ( level & ( MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError | MT_Logger_ABC::eLogLevel_Warning ) ) != 0;
    };
}


// -----------------------------------------------------------------------------
// Name: Logger::LogString
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::LogString( const char* /*szLayerName*/, E_LogLevel nLevel, const char* szMsg, const char* szContext, int /*code*/ )
{
    ValuedListItem* pItem = factory_.CreateItem( this );
    pItem->setText( 0, GetTimestampAsString() );
    pItem->setText( 1, szMsg );
    const QColor itemColor = ChooseMessageColor( nLevel );
    pItem->SetFontColor( itemColor );

    if( szContext != 0 )
    {
        ValuedListItem* pSubItem = factory_.CreateItem( pItem );
        pSubItem->setText( 1, szContext );
        pSubItem->setMultiLinesEnabled( true );
        pSubItem->SetFontColor( itemColor );
    }

    if( IsErrorMesssage( nLevel ) )
        emit Error();
}


// -----------------------------------------------------------------------------
// Name: Logger::OnRequestPopup
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::OnRequestPopup( QListViewItem* /*pItem*/, const QPoint& pos )
{
    popupMenu_.popup( pos );
}
