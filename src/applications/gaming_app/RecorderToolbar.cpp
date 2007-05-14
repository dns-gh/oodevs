// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "RecorderToolbar.h"
#include "moc_RecorderToolbar.cpp"
#include "gaming/Network.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/MsgRecorder.h"
#include "clients_gui/resources.h"

#include <fstream>

// -----------------------------------------------------------------------------
// Name: RecorderToolbar constructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
RecorderToolbar::RecorderToolbar( QMainWindow* pParent, Network& network, QObject* browser )
    : QToolBar    ( pParent )
    , msgRecorder_( *new MsgRecorder( network ) )
{
    setCaption( tr("Order recorder") );
    playButton_ = new QToolButton( QPixmap( xpm_recplay ), tr( "Replay order file" ), "", browser, SLOT( Load() ), this );
    saveButton_ = new QToolButton( QPixmap( xpm_save )   , tr( "Save" ),              "", browser, SLOT( Save() ), this );
    recButton_  = new QToolButton( QPixmap( xpm_recrec ) , tr( "Start recording" ),   "", this, SLOT( ToggleRecord() ), this );
    saveButton_->setEnabled( false );

    connect( browser, SIGNAL( OrderOpened( const QString& ) ), SLOT( Play( const QString& ) ) );
    connect( browser, SIGNAL( OrderSaved ( const QString& ) ), SLOT( Save( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: RecorderToolbar destructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
RecorderToolbar::~RecorderToolbar()
{
    delete &msgRecorder_;
}

// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Play
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::Play( const QString& filename )
{
    if( filename.isEmpty() )
        return;
    msgRecorder_.Play( filename.ascii() );
    saveButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: RecorderToolbar::ToggleRecord
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::ToggleRecord()
{
    const bool wasRecording = msgRecorder_.Stop();
    saveButton_->setEnabled( wasRecording );
    playButton_->setEnabled( wasRecording );
    if( wasRecording )
    {
        recButton_->setPixmap( QPixmap( xpm_recrec ) );
        recButton_->setTextLabel( tr( "Start recording" ) );
    }
    else
    {
        msgRecorder_.Record();
        recButton_->setPixmap( QPixmap( xpm_recstop ) );
        recButton_->setTextLabel( tr( "Stop recording" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Save
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
void RecorderToolbar::Save( const QString& filename )
{
    if( filename.isEmpty() )
        return;
    msgRecorder_.Write( filename.ascii() );
    saveButton_->setEnabled( false );
}
