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
#include "Application.h"
#include "moc_TrayMenu.cpp"
#include "clients_kernel/Tools.h"
#include <QtGui/QMainWindow>

// -----------------------------------------------------------------------------
// Name: TrayMenu constructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
TrayMenu::TrayMenu( QWidget* mainWindow )
    : main_( mainWindow )
{
    setPalette( mainWindow->palette() );
    QAction* showAction = new QAction( tools::translate( "TrayMenu", "Show Normal" ), this );
    connect( showAction, SIGNAL( triggered() ), mainWindow, SLOT( showNormal() ) );
    QAction* quitAction = new QAction( tools::translate( "TrayMenu", "Quit" ), this );
    connect( quitAction, SIGNAL( triggered() ), this, SLOT( OnQuit() ) );
    addAction( showAction );
    addAction( quitAction );
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
    main_->showNormal();
    qApp->closeAllWindows();
}
