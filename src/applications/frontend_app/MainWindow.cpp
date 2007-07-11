// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "MainWindow.h"
#include "ExercisesTab.h"
#include "TerrainsTab.h"
#include "DatasetsTab.h"
#include "LoginDialog.h"
#include "frontend/Profile.h"
#include "frontend/Networker.h"
#include "clients_kernel/Controllers.h"
#include <qtabwidget.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow( kernel::Controllers& controllers, frontend::Networker& networker, const frontend::Profile& profile )
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , controllers_( controllers )
    , networker_( networker )
    , profile_( profile )
    , loginDialog_( new LoginDialog( this, profile_, networker_ ) )
{
    setCaption( APP_NAME );
    setFixedSize( 600, 400 );
    const QRect& screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - QPoint( width() / 2, height() / 2 ) );

    QTabWidget* tabs = new QTabWidget( this );
    tabs->addTab( new ExercisesTab( tabs, controllers_ ), tr( "Exercises" ) );
    tabs->addTab( new TerrainsTab ( tabs, controllers_ ), tr( "Terrains" ) );
    tabs->addTab( new DatasetsTab ( tabs, controllers_ ), tr( "Datasets" ) );
    setCentralWidget( tabs );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2007-01-30
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const frontend::Networker& networker )
{
    if( networker.IsConnected() )
        loginDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MainWindow::NotifyUpdated
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void MainWindow::NotifyUpdated( const frontend::Profile& profile )
{
    if( !profile.IsLoggedIn() )
        loginDialog_->show();
}
