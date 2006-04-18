// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"

#include "MT/MT_Logger/MT_logger_lib.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent )
    : QListView( pParent )
    , MT_Logger_ABC ()
{
    layers_[ (E_DataFlow)-1 ] = new sLoggerLayer( (E_DataFlow)-1, Qt::black     );
    layers_[ eDefault       ] = new sLoggerLayer( eReceived     , Qt::black     );
    layers_[ eReceived      ] = new sLoggerLayer( eReceived     , Qt::darkBlue  );
    layers_[ eSent          ] = new sLoggerLayer( eSent         , Qt::darkGreen );

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
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        delete it->second;
}


// -----------------------------------------------------------------------------
// Name: Logger::LogString
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::LogString( const char* /*szLayerName*/, E_LogLevel /*nLevel*/, const char* szMsg, const char* szContext, int code )
{
    const sLoggerLayer* layer = layers_[ (E_DataFlow)code ];
    if( !layer )
        throw std::runtime_error( "Unregistered logging layer" );

    // lastItem() ?  firstChild();
    ValuedListItem* pItem = new ValuedListItem( layer, this, GetTimestampAsString(), szMsg );
    pItem->SetFontColor( layer->color_ );

    if( szContext != 0 )
    {
        ValuedListItem* pSubItem = new ValuedListItem( layer, pItem, "", szContext );
        pSubItem->setMultiLinesEnabled( true );
        pSubItem->SetFontColor( layer->color_ );
    }
}


// -----------------------------------------------------------------------------
// Name: Logger::OnRequestPopup
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::OnRequestPopup( QListViewItem* /*pItem*/, const QPoint& pos )
{
    popupMenu_.popup( pos );
}
