// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/RecorderToolbar.cpp $
// $Author: Ape $
// $Modtime: 22/06/05 15:59 $
// $Revision: 3 $
// $Workfile: RecorderToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "RecorderToolbar.h"
#include "moc_RecorderToolbar.cpp"

#include "Network.h"
#include "MsgRecorder.h"
#include "App.h"
#include "AgentServerMsgMgr.h"
#include "resources.h"

//#include "res/recrec.xpm"
//#include "res/recstop.xpm"
//#include "res/recplay.xpm"

// -----------------------------------------------------------------------------
// Name: RecorderToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
RecorderToolbar::RecorderToolbar( QMainWindow* pParent )
    : QToolBar      ( pParent )
    , msgRecorder_  ( App::GetApp().GetNetwork().GetMessageMgr().GetMsgRecorder() )
{
    this->setCaption( tr("Enregisteur d'ordres") );
    // $$$$ AGE 2005-03-14: icons
    pPlayButton_ = new QToolButton( QPixmap( xpm_recplay ), tr( "Rejouer un fichier d'ordres" ), "", this, SLOT( Play() ),   this );
    pStopButton_ = new QToolButton( QPixmap( xpm_recstop ), tr( "Stop" ),                        "", this, SLOT( Stop() ), this );
    pRecButton_ =  new QToolButton( QPixmap( xpm_recrec ),  tr( "Enregister des ordres" ),       "", this, SLOT( Record() ),   this );

    pStopButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: RecorderToolbar destructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
RecorderToolbar::~RecorderToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Play
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::Play()
{
    QString strFileName = QFileDialog::getOpenFileName( QString::null, tr( "Fichiers d'ordres (*.ord)" ), 0, 0, tr("Ouvrir") );
    if( strFileName == QString::null )
        return;

    MT_BinaryInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strFileName.ascii() );
    msgRecorder_.ReadArchive( archive );

    msgRecorder_.Play();

//    pPlayButton_->setEnabled( false );
//    pRecButton_->setEnabled( false );
//    pStopButton_->setEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: RecorderToolbar::Stop
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void RecorderToolbar::Stop()
{
    if( msgRecorder_.GetState() == MsgRecorder::eRecording )
    {
        msgRecorder_.Stop();
        QString strFileName = QFileDialog::getSaveFileName( QString::null, tr( "Fichiers d'ordres (*.ord)" ), 0, 0, tr("Enregistrer") );
        if( strFileName == QString::null )
            return;
        if( strFileName.right( 4 ) != ".ord" )
            strFileName += ".ord";
        MT_BinaryOutputArchive archive;
        archive.EnableExceptions( true );
        msgRecorder_.WriteArchive( archive );
        archive.WriteToFile( strFileName.ascii() );
    }
    else
        msgRecorder_.Stop();

    pPlayButton_->setEnabled( true );
    pRecButton_->setEnabled( true );
    pStopButton_->setEnabled( false );
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

