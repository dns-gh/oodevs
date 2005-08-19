//****************************************************************************
//
// $Created:  NLD 2002-01-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogWindow.cpp $
// $Author: Nld $
// $Modtime: 27/10/03 17:29 $
// $Revision: 6 $
// $Workfile: MOS_LogWindow.cpp $
//
//*****************************************************************************


#include "MOS_Light_pch.h"
#include "MOS_LogWindow.h"
#include "moc_MOS_LogWindow.cpp"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qheader.h>

//-----------------------------------------------------------------------------
// Name: MOS_LogWindow constructor
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
MOS_LogWindow::MOS_LogWindow( QWidget* pParent, uint nLogLevels, uint nLogLayers )
    : QListView    ( pParent )
    , MT_Logger_ABC( nLogLevels, nLogLayers )
    , bAutoScroll_ ( true )
    , nNbrLinesMax_( 1000 )
{
    setCaption( "Log window" );        
//    resize( 640, 300 );

    // Columns
    addColumn( tr("Date") );
    addColumn( tr("Layer") );
    addColumn( tr("Level") );
    addColumn( tr("Message") );
    addColumn( tr("Context") );
    addColumn( tr("Code") );

    // Properties
    setAllColumnsShowFocus( TRUE );
    setSelectionMode( NoSelection );
    setSorting( -1 ); // No sorting
    header()->setMovingEnabled( TRUE );
    header()->setClickEnabled( TRUE );

    // Menu
    pMenu_ = new QPopupMenu( this );
    pMenu_->setCheckable( true );
    connect( this, SIGNAL( rightButtonPressed  ( QListViewItem*, const QPoint&, int ) ), 
             this, SLOT( SlotRightButtonPressed( QListViewItem*, const QPoint&, int ) ) );

    // Action : auto scroll
    QAction* pAction = new QAction( tr("&Auto scroll"), tr("&Auto scroll"), 0, this, 0, true );
	connect( pAction, SIGNAL( activated() ), SLOT( SlotSetAutoScroll() ) );
    pAction->addTo( pMenu_ );
    pAction->setOn( true );
    
    
    MT_LOG_REGISTER_LOGGER( *this );
    SetTimestampFormat( "%H:%M:%S" );
}

//-----------------------------------------------------------------------------
// Name: MOS_LogWindow destructor
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
MOS_LogWindow::~MOS_LogWindow()
{
    MT_LOG_UNREGISTER_LOGGER( *this );
}

//=============================================================================
// LOG METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_LOGConsole::LogString
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
void MOS_LogWindow::LogString( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int /*nCode*/ )
{
    QListViewItem* pItem = NULL;

    if( childCount() > nNbrLinesMax_ )
    {
        pItem = firstChild();
        assert( pItem );
        pItem->moveItem( lastItem() );
        pItem->setText( 0, GetTimestampAsString()        ); // Date 
        pItem->setText( 1, strLayerName                  ); // Layer
        pItem->setText( 2, GetLogLevelAsString( nLevel ) ); // Level
        pItem->setText( 3, szMsg                         ); // Message
        pItem->setText( 4, strContext                    ); // Context
        

    }
    else
    {
        pItem = new QListViewItem( this, lastItem(), GetTimestampAsString()         // Date 
                                                   , strLayerName                   // Layer
                                                   , GetLogLevelAsString( nLevel )  // Level
                                                   , szMsg                          // Message
                                                   , strContext                     // Context
                                 );                                                    
    }

    
    
    
    if( bAutoScroll_ )
    {
        setCurrentItem   ( pItem );
        ensureItemVisible( pItem );
    }
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LogWindow::SlotRightButtonPressed
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
void MOS_LogWindow::SlotRightButtonPressed( QListViewItem* /*pItem*/, const QPoint &pos, int )
{
    pMenu_->popup( pos );
}
//-----------------------------------------------------------------------------
// Name: MOS_LogWindow::SlotSetAutoScroll
// Created:  NLD 2002-01-07 
//-----------------------------------------------------------------------------
void MOS_LogWindow::SlotSetAutoScroll()
{
    bAutoScroll_ = !bAutoScroll_;
}

//=============================================================================
// QT EVENTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LogWindow::hideEvent
// Created:  NLD 2002-01-18 
//-----------------------------------------------------------------------------
void MOS_LogWindow::hideEvent( QHideEvent* /*pEvent*/ )
{
//    show();    
}

