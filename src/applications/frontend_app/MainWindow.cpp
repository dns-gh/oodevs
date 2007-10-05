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
#include "ActionList.h"
#include "MainMenu.h"
#include "TerrainCreationPanel.h"
#include "StartExercisePanel.h"
#include "StartAnalysePanel.h"
#include "resources.h"
#include "tools/GeneralConfig.h"

#include <qapplication.h>
#include <qaction.h>
#include <qwidgetstack.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
    , config_( new tools::GeneralConfig() )
{
    config_->Parse( qApp->argc(), qApp->argv() );
    setCaption( APP_NAME );
    setMinimumSize( 640, 500 );

    menu_ = new MainMenu( this );

    QHBox* box = new QHBox( this );
    list_ = new ActionList( box );
    box->setStretchFactor( list_, 1 );

    pages_ = new QWidgetStack( box );
    pages_->addWidget( new QWidget( this ) );
    box->setStretchFactor( pages_, 3 );

    AddAction< TerrainCreationPanel >( tr( "Prepare" ), *new QAction( tr( "Create terrain" ) , MAKE_PIXMAP( terrain_create ), "Create terrain" , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Prepare" ), *new QAction( tr( "Create exercise" ), MAKE_PIXMAP( data_create )   , "Create exercise", 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Prepare" ), *new QAction( tr( "Edit exercise" )  , MAKE_PIXMAP( data_create )   , "Edit exercise"  , 0, this, 0, true ) );

    AddAction< StartExercisePanel >( tr( "Play" ), *new QAction( tr( "Start game" )  , MAKE_PIXMAP( terrain_create ), "Start game"   , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Play" ), *new QAction( tr( "Restart game" ), MAKE_PIXMAP( terrain_create ), "Restart game" , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Play" ), *new QAction( tr( "Join game" )   , MAKE_PIXMAP( terrain_create ), "Join game"    , 0, this, 0, true ) );

    AddAction< StartAnalysePanel >( tr( "Analyse" ), *new QAction( tr( "Start analyse" ) , MAKE_PIXMAP( terrain_create ), "Start analyse"  , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Analyse" ), *new QAction( tr( "Join analyse" ), MAKE_PIXMAP( terrain_create ), "Join game" , 0, this, 0, true ) );

    statusBar();

    setCentralWidget( box );
    CenterWindow();
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MainWindow::CenterWindow
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void MainWindow::CenterWindow()
{
    const QRect& screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - QPoint( width() / 2, height() / 2 ) );
}

// -----------------------------------------------------------------------------
// Name: MainWindow::AddAction
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
template< typename Page >
void MainWindow::AddAction( const QString& category, QAction& action )
{
    menu_->AddAction( category, action );
    list_->AddAction( category, action );
    pages_->addWidget( new Page( pages_, action, *config_ ) );
}
