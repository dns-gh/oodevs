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
    pPlayButton_ = new QToolButton( QPixmap( xpm_recplay ), tr( "Replay order file" ), "", this, SLOT( Play() ),   this );
    pStopButton_ = new QToolButton( QPixmap( xpm_recstop ), tr( "Stop" ),              "", this, SLOT( Stop() ), this );
    pRecButton_ =  new QToolButton( QPixmap( xpm_recrec ),  tr( "Start recording" ),   "", this, SLOT( Record() ),   this );

    pStopButton_->setEnabled( false );
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
    QString strFileName = QFileDialog::getOpenFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Open" ) );
    if( strFileName == QString::null )
        return;

    msgRecorder_.Play( strFileName.ascii() );
}

// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Stop
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::Stop()
{
    if( msgRecorder_.Stop() )
    {
        pPlayButton_->setEnabled( true );
        pRecButton_->setEnabled( true );
        pStopButton_->setEnabled( false );
        QString strFileName = QFileDialog::getSaveFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
        if( strFileName == QString::null )
            return;
        if( strFileName.right( 4 ) != ".ord" )
            strFileName += ".ord";
        msgRecorder_.Write( strFileName.ascii() );
    }
}


// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Record
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::Record()
{
    msgRecorder_.Record();
    pPlayButton_->setEnabled( false );
    pRecButton_->setEnabled( false );
    pStopButton_->setEnabled( true );
}

