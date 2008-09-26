// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "TrayMenu.h"
#include "Session.h" 
#include "moc_TrayMenu.cpp" 

// -----------------------------------------------------------------------------
// Name: TrayMenu constructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
TrayMenu::TrayMenu( QWidget* mainWindow, boost::shared_ptr< Session > sessionStatus )
    : sessionStatus_ ( sessionStatus ) 
{
    setPalette( mainWindow->palette() ) ; 
    insertItem( tr( "Show Normal") , mainWindow, SLOT(showNormal()) );
    insertSeparator();
    QPopupMenu* simMenu = new QPopupMenu( this ); 
    simMenu->setPalette( palette() ) ; 
    nSimId_ = insertItem( tr( "Simulation" ), simMenu ) ;
    setItemEnabled( nSimId_, false ) ; 
    simMenu->insertItem( tr( "Close") , this, SLOT( OnCloseSim() ) ) ; 
    QPopupMenu* guiMenu = new QPopupMenu( this ); 
    guiMenu->setPalette( palette() ) ; 
    nGUIId_ = insertItem( tr( "GUI" ), guiMenu ) ; 
    setItemEnabled( nGUIId_, false ) ; 
    guiMenu->insertItem( tr( "Close")  ,this, SLOT( OnCloseGUI() ) ) ; 
    insertSeparator();
    insertItem( tr( "Quit") , this , SLOT( OnQuit() ) );
    QObject::connect(this, SIGNAL( aboutToShow() ), this, SLOT(OnAboutToShow() ) );
}

// -----------------------------------------------------------------------------
// Name: TrayMenu destructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
TrayMenu::~TrayMenu()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrayMenu::OnAboutToShow
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
void TrayMenu::OnAboutToShow()
{
    Update(); 
}

// -----------------------------------------------------------------------------
// Name: TrayMenu::Update
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
void TrayMenu::Update()
{
    setItemEnabled( nSimId_, sessionStatus_->IsSimRunning() ) ;                      
    setItemEnabled( nGUIId_, sessionStatus_->IsGUIRunning() ) ;                      
}

// -----------------------------------------------------------------------------
// Name: TrayMenu::OnCloseSim
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void TrayMenu::OnCloseSim()
{
    sessionStatus_->StopSimulation( true ); 
}

// -----------------------------------------------------------------------------
// Name: TrayMenu::OnCloseGUI
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void TrayMenu::OnCloseGUI()
{
    sessionStatus_->StopGUI( true ); 
}


// -----------------------------------------------------------------------------
// Name: TrayMenu::OnQuit
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void TrayMenu::OnQuit()
{
    qApp->mainWidget()->showNormal();
    qApp->closeAllWindows(); 
}
