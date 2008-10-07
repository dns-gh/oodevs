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
#include "moc_TrayMenu.cpp"
#include "Session.h" 
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: TrayMenu constructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
TrayMenu::TrayMenu( QWidget* mainWindow )
    : currentSession_( 0 )
{
    setPalette( mainWindow->palette() );
    insertItem( tools::translate( "TrayMenu", "Show Normal" ), mainWindow, SLOT( showNormal() ) );
    insertItem( tools::translate( "TrayMenu", "Quit" ), this, SLOT( OnQuit() ) );
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
// Name: TrayMenu::OnQuit
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void TrayMenu::OnQuit()
{
    qApp->mainWidget()->showNormal();
    qApp->closeAllWindows();
}
