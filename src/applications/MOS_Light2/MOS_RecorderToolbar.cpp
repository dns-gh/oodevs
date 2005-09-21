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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RecorderToolbar.cpp $
// $Author: Ape $
// $Modtime: 22/06/05 15:59 $
// $Revision: 3 $
// $Workfile: MOS_RecorderToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_RecorderToolbar.h"
#include "moc_MOS_RecorderToolbar.cpp"

#include "MOS_MsgRecorder.h"
#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerMsgMgr.h"
#include "resources.h"

//#include "res/recrec.xpm"
//#include "res/recstop.xpm"
//#include "res/recplay.xpm"

// -----------------------------------------------------------------------------
// Name: MOS_RecorderToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_RecorderToolbar::MOS_RecorderToolbar( QMainWindow* pParent )
    : QToolBar      ( pParent )
    , msgRecorder_  ( MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().GetMsgRecorder() )
{
    this->setCaption( tr("Enregisteur d'ordres") );
    // $$$$ AGE 2005-03-14: icons
    pPlayButton_ = new QToolButton( QPixmap( xpm_circle ), tr( "Rejouer un fichier d'ordres" ), "", this, SLOT( Play() ),   this );
    pStopButton_ = new QToolButton( QPixmap( xpm_circle ), tr( "Stop" ),                        "", this, SLOT( Stop() ), this );
    pRecButton_ =  new QToolButton( QPixmap( xpm_circle ),  tr( "Enregister des ordres" ),       "", this, SLOT( Record() ),   this );

    pStopButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_RecorderToolbar destructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_RecorderToolbar::~MOS_RecorderToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_RecorderToolbar::Play
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void MOS_RecorderToolbar::Play()
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
// Name: MOS_RecorderToolbar::Stop
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void MOS_RecorderToolbar::Stop()
{
    if( msgRecorder_.GetState() == MOS_MsgRecorder::eRecording )
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
// Name: MOS_RecorderToolbar::Record
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
void MOS_RecorderToolbar::Record()
{
    msgRecorder_.Record();
    pPlayButton_->setEnabled( false );
    pRecButton_->setEnabled( false );
    pStopButton_->setEnabled( true );
}

