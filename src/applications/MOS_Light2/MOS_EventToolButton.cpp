// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_EventToolButton.cpp $
// $Author: Ape $
// $Modtime: 4/10/04 16:54 $
// $Revision: 1 $
// $Workfile: MOS_EventToolButton.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_EventToolButton.h"
#include "moc_MOS_EventToolButton.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton constructor
/** @param  iconSet 
    @param  toolTip 
    @param  pParent 
    @param  nFilterId 
*/
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
MOS_EventToolButton::MOS_EventToolButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* pParent, int nFilterId )
    : QToolButton   ( iconSet, "0", "", 0, 0, pParent )
    , nFilterId_    ( nFilterId )
    , timer_        ( *new QTimer( this ) )
    , toolTip_      ( toolTip )
{
    this->setUsesTextLabel( true );
    this->setTextPosition( QToolButton::BesideIcon );

    // If we are filtering events, display the tooltip rather than the number of events received.
    if( nFilterId_ != -1 )
    {
        // Use a small font.
//        QFont font = this->font();
//        font.setPointSize( 6 );
//        this->setFont( font );
        this->setTextLabel( toolTip_ );
    }

    QToolTip::add( this, toolTip );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),    this, SLOT( ClearEvents() ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( ClearEvents() ) );
    connect( this, SIGNAL( clicked() ), this, SLOT( OnClicked() ) );

    connect( &timer_, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::SetSignalsSlots
/** @param  pEmitter 
    @param  pNewEventSignal 
    @param  pEventViewedSignal 
    @param  pFocusOnEventSlot 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolButton::SetSignalsSlots( QObject* pEmitter, const char* pNewEventSignal, const char* pEventViewedSignal, const char* pFocusOnEventSlot )
{
    assert( pEmitter != 0 && pNewEventSignal != 0 && pEventViewedSignal != 0 && pFocusOnEventSlot != 0 );
    connect( pEmitter, pNewEventSignal,               this,     SLOT( OnNewEvent( int ) ) );
    connect( pEmitter, pEventViewedSignal,            this,     SLOT( OnEventViewed( int ) ) );
    connect( this,     SIGNAL( FocusOnEvent( int, bool ) ), pEmitter, pFocusOnEventSlot );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton destructor
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
MOS_EventToolButton::~MOS_EventToolButton()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::OnNewEvent
/** @param  agent 
*/
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolButton::OnNewEvent( int nEventId )
{
    // If we are filtering the events, make sure it's the right one.
    if( nFilterId_ != -1 && nEventId != nFilterId_ )
        return;

    // Avoid keeping multiple instances of the event in the list.
    IT_IntVector it = std::find( currentEvents_.begin(), currentEvents_.end(), nEventId );
    if( it != currentEvents_.end() )
        currentEvents_.erase( it );

    currentEvents_.push_back( nEventId );

    // If we are not filtering, display the number of events received.
    if( nFilterId_ == -1 )
    {
        this->setTextLabel( QString::number( currentEvents_.size() ) );
    
        // Restore the tooltip (it was destroyed by the setTextLabel).
        QToolTip::add( this, toolTip_ );
    }

    // Flash the number red.
    this->setPaletteForegroundColor( Qt::red );
    timer_.stop();
    timer_.start( 1500, true );

    // And put the label in bold.
    QFont font = this->font();
    font.setBold( true );
    this->setFont( font );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::OnEventViewed
/** @param  agent 
*/
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolButton::OnEventViewed( int nEventId )
{
    // Remove the event from the list.
    IT_IntVector it = std::find( currentEvents_.begin(), currentEvents_.end(), nEventId );
    if( it != currentEvents_.end() )
        currentEvents_.erase( it );

    // If we are not filtering, display the number of events left.
    if( nFilterId_ == -1 )
    {
        this->setTextLabel( QString::number( currentEvents_.size() ) );

        // Restore the tooltip (it was destroyed by the setTextLabel).
        QToolTip::add( this, toolTip_ );
    }

    // And put the label in normal type if there are no more events.
    if( currentEvents_.empty() )
    {
        QFont font = this->font();
        font.setBold( false );
        this->setFont( font );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::ClearEvents
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolButton::ClearEvents()
{
    currentEvents_.clear();
    // If we are not filtering, display the number of events received (0 here).
    if( nFilterId_ == -1 )
    {
        this->setTextLabel( "0" );

        // Restore the tooltip (it was destroyed by the setTextLabel).
        QToolTip::add( this, toolTip_ );
    }

     // And put the label in normal type.
    QFont font = this->font();
    font.setBold( false );
    this->setFont( font );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::OnClicked
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolButton::OnClicked()
{
    if( currentEvents_.empty() )
        return;

    // Get the keyboard state.
    BYTE stateBuffer[256];
    GetKeyboardState(stateBuffer);

    // If pressed with shift, clear the msg list.
    if( stateBuffer[VK_SHIFT]   & 0x80 )
    {
        this->ClearEvents();
        return;
    }

    int nEventId = currentEvents_.back();
    currentEvents_.pop_back();
    currentEvents_.insert( currentEvents_.begin(), nEventId );

    // Request focusing on the event, and request centering if the control key is pressed.
    emit FocusOnEvent( nEventId, stateBuffer[VK_CONTROL] & 0x80 );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolButton::OnTimer
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolButton::OnTimer()
{
    this->setPaletteForegroundColor( Qt::black );
}
