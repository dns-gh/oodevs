// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-02 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Logger.cpp $
// $Author: Ape $
// $Modtime: 23/07/04 17:48 $
// $Revision: 3 $
// $Workfile: Logger.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"

#include "Types.h"
#include "MT/MT_Logger/MT_logger_lib.h"
#include "MT/MT_Qt/MT_ValuedListViewItem.h"

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent )
    : QListView( pParent )
    , MT_Logger_ABC ()
{
    MT_LogManager::Instance().RegisterLogger( *this );

    this->setMinimumSize( 1, 1 );
    this->setShowSortIndicator( true );
    //this->setSorting( 0, false );
    this->setSorting( -1 );
    this->setRootIsDecorated( true );
    this->addColumn( tr( "Recu" ) );
    this->addColumn( tr( "Log" ) );
    this->setResizeMode( QListView::LastColumn );
    this->setAllColumnsShowFocus ( true );

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


// -----------------------------------------------------------------------------
// Name: Logger::LogString
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::LogString( const char* /*szLayerName*/, E_LogLevel /*nLevel*/, const char* szMsg, const char* szContext, int nCode )
{
    // lastItem() ?  firstChild();
    MT_ValuedListViewItem<E_DataFlow>* pItem = new MT_ValuedListViewItem<E_DataFlow>( (E_DataFlow)nCode, this, this->GetTimestampAsString(), szMsg );
    if( nCode == eReceived )
        pItem->SetFontColor( Qt::darkBlue );
    else if( nCode == eSent )
        pItem->SetFontColor( Qt::darkGreen );

    if( szContext != 0 )
    {
        MT_ValuedListViewItem<E_DataFlow>* pSubItem = new MT_ValuedListViewItem<E_DataFlow>( (E_DataFlow)nCode, pItem, "", szContext );
        pSubItem->setMultiLinesEnabled( true );
        if( nCode == eReceived )
            pSubItem->SetFontColor( Qt::darkBlue );
        else if( nCode == eSent )
            pSubItem->SetFontColor( Qt::darkGreen );
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
