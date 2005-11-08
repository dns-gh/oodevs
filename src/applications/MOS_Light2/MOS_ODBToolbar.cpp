// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-30 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ODBToolbar.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: MOS_ODBToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ODBToolbar.h"
#include "moc_MOS_ODBToolbar.cpp"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_ObjectManager.h"
#include "MOS_MainWindow.h"


// -----------------------------------------------------------------------------
// Name: MOS_ODBToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
MOS_ODBToolbar::MOS_ODBToolbar( QMainWindow* pParent )
    : QToolBar( pParent )
{
    this->setLabel( tr( "ODB" ) );

    QAction* pLoadAction   = new QAction( tr("&Ouvrir"),          MAKE_ICON( open ),   tr("&Ouvrir un ODB"),         CTRL + Key_O, this );
    QAction* pSaveAction   = new QAction( tr("Enregi&ster"),      MAKE_ICON( save ),   tr("&Enregister l'ODB"),      CTRL + Key_S, this );
    QAction* pSaveAsAction = new QAction( tr("En&register sous"), MAKE_ICON( saveas ), tr("&Enregister l'ODB sous"), CTRL + Key_R, this );
    pLoadAction->addTo( this );
    pSaveAction->addTo( this );
    pSaveAsAction->addTo( this );

    connect( pLoadAction,   SIGNAL( activated() ), this,  SLOT( OnOpen() ) );
    connect( pSaveAction,   SIGNAL( activated() ), this,  SLOT( OnSave() ) );
    connect( pSaveAsAction, SIGNAL( activated() ), this,  SLOT( OnSaveAs() ) );

    connect( this, SIGNAL( ODBLoaded() ), & MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ODBToolbar destructor
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
MOS_ODBToolbar::~MOS_ODBToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ODBToolbar::OnOpen
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_ODBToolbar::OnOpen()
{
    QString strFilename = QFileDialog::getOpenFileName( QString::null, "*.xml", 0 );
    if( strFilename == QString::null )
        return;

    MOS_App::GetApp().GetAgentManager().DeleteAllAgents();
    MOS_App::GetApp().GetAgentManager().DeleteAllGtias();
    MOS_App::GetApp().GetObjectManager().DeleteAllObjects();
    MOS_App::GetApp().ReadODB( strFilename.ascii() );
    emit ODBLoaded();
}


// -----------------------------------------------------------------------------
// Name: MOS_ODBToolbar::OnSave
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_ODBToolbar::OnSave()
{
    MOS_App::GetApp().WriteODB();
}


// -----------------------------------------------------------------------------
// Name: MOS_ODBToolbar::OnSaveAs
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_ODBToolbar::OnSaveAs()
{
    QString strFilename = QFileDialog::getSaveFileName( QString::null, "*.xml", 0 );
    if( strFilename == QString::null )
        return;

    MOS_App::GetApp().WriteODB( strFilename.ascii() );
}
