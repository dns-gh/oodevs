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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Logger.cpp $
// $Author: Ape $
// $Modtime: 23/07/04 17:48 $
// $Revision: 3 $
// $Workfile: MOS_Logger.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_Logger.h"
#include "moc_MOS_Logger.cpp"

#include "MOS_Types.h"

// -----------------------------------------------------------------------------
// Name: MOS_Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
MOS_Logger::MOS_Logger( QWidget* pParent )
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
// Name: MOS_Logger destructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
MOS_Logger::~MOS_Logger()
{
    MT_LogManager::Instance().UnregisterLogger( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_Logger::LogString
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void MOS_Logger::LogString( const char* /*szLayerName*/, E_LogLevel /*nLevel*/, const char* szMsg, const char* szContext, int nCode )
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
// Name: MOS_Logger::OnRequestPopup
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void MOS_Logger::OnRequestPopup( QListViewItem* /*pItem*/, const QPoint& pos )
{
    popupMenu_.popup( pos );
}
