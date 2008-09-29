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
TrayMenu::TrayMenu( QWidget* mainWindow )
    : currentSession_ ( NULL ) 
{
    setPalette( mainWindow->palette() ) ; 
    insertItem( tr( "Show Normal") , mainWindow, SLOT(showNormal()) );
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
