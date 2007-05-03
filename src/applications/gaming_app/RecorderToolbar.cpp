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
RecorderToolbar::RecorderToolbar( QMainWindow* pParent, Network& network )
    : QToolBar    ( pParent )
    , msgRecorder_( *new MsgRecorder( network ) )
{
    setCaption( tr("Order recorder") );
    playButton_ = new QToolButton( QPixmap( xpm_recplay ), tr( "Replay order file" ), "", this, SLOT( Play() ), this );
    recButton_  = new QToolButton( QPixmap( xpm_recrec ) , tr( "Start recording" ),   "", this, SLOT( ToggleRecord() ), this );
    saveButton_ = new QToolButton( QPixmap( xpm_save )   , tr( "Save" ),              "", this, SLOT( Save() ), this );
    saveButton_->setEnabled( false );
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
void RecorderToolbar::Play()
{
    QString filename = QFileDialog::getOpenFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Open" ) );
    if( filename == QString::null )
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
void RecorderToolbar::Save()
{
    QString filename = QFileDialog::getSaveFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
    if( filename == QString::null )
        return;
    if( filename.right( 4 ) != ".ord" )
        filename += ".ord";
    msgRecorder_.Write( filename.ascii() );
    saveButton_->setEnabled( false );
}
