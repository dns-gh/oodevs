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
#include "resources.h"

#include <qapplication.h>
#include <qaction.h>
#include <qwidgetstack.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
{
    setCaption( APP_NAME );
    setMinimumSize( 600, 400 );

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

    AddAction< TerrainCreationPanel >( tr( "Play" ), *new QAction( tr( "Start game" )  , MAKE_PIXMAP( terrain_create ), "Start game"   , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Play" ), *new QAction( tr( "Restart game" ), MAKE_PIXMAP( terrain_create ), "Restart game" , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Play" ), *new QAction( tr( "Join game" )   , MAKE_PIXMAP( terrain_create ), "Join game"    , 0, this, 0, true ) );

    AddAction< TerrainCreationPanel >( tr( "Analyse" ), *new QAction( tr( "Replay game" ) , MAKE_PIXMAP( terrain_create ), "Replay game"  , 0, this, 0, true ) );
    AddAction< TerrainCreationPanel >( tr( "Analyse" ), *new QAction( tr( "Analyse game" ), MAKE_PIXMAP( terrain_create ), "Analyse game" , 0, this, 0, true ) );

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
    pages_->addWidget( new Page( pages_, action ) );
}
