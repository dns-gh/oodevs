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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ODBToolbar.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: ODBToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ODBToolbar.h"
#include "moc_ODBToolbar.cpp"

#include "App.h"
#include "AgentManager.h"
#include "ObjectManager.h"
#include "MainWindow.h"


// -----------------------------------------------------------------------------
// Name: ODBToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
ODBToolbar::ODBToolbar( QMainWindow* pParent )
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

    connect( this, SIGNAL( ODBLoaded() ), & MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ) );
}


// -----------------------------------------------------------------------------
// Name: ODBToolbar destructor
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
ODBToolbar::~ODBToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: ODBToolbar::OnOpen
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void ODBToolbar::OnOpen()
{
    QString strFilename = QFileDialog::getOpenFileName( QString::null, "*.xml", 0 );
    if( strFilename == QString::null )
        return;

    App::GetApp().GetAgentManager().DeleteAllAgents();
    App::GetApp().GetAgentManager().DeleteAllGtias();
    App::GetApp().GetObjectManager().DeleteAllObjects();
    App::GetApp().ReadODB( strFilename.ascii() );
    emit ODBLoaded();
}


// -----------------------------------------------------------------------------
// Name: ODBToolbar::OnSave
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void ODBToolbar::OnSave()
{
    App::GetApp().WriteODB();
}


// -----------------------------------------------------------------------------
// Name: ODBToolbar::OnSaveAs
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void ODBToolbar::OnSaveAs()
{
    QString strFilename = QFileDialog::getSaveFileName( QString::null, "*.xml", 0 );
    if( strFilename == QString::null )
        return;

    App::GetApp().WriteODB( strFilename.ascii() );
}
