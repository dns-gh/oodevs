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
#include "resources.h"

#include <qapplication.h>
#include <qaction.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
{
    setCaption( APP_NAME );
    setFixedSize( 600, 400 );
    
    QHBox* box = new QHBox( this );
    {
        ActionList* list = new ActionList( box );
        box->setStretchFactor( list, 1 );

        list->AddAction( tr( "Prepare" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Create terrain" , 0, list ) );
        list->AddAction( tr( "Prepare" ), *new QAction( MAKE_PIXMAP( data_create )   , "Create exercise", 0, list ) );
        list->AddAction( tr( "Prepare" ), *new QAction( MAKE_PIXMAP( data_create )   , "Edit exercise"  , 0, list ) );

        list->AddAction( tr( "Play" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Start game"   , 0, list ) );
        list->AddAction( tr( "Play" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Restart game" , 0, list ) );
        list->AddAction( tr( "Play" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Join game"    , 0, list ) );

        list->AddAction( tr( "Analyse" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Replay game"  , 0, list ) );
        list->AddAction( tr( "Analyse" ), *new QAction( MAKE_PIXMAP( terrain_create ), "Analyse game" , 0, list ) );
    }
    QWidget* widget = new QWidget( box ); // $$$$ SBO 2007-10-04: dummy
    box->setStretchFactor( widget, 3 );

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
