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
#include "ExerciseList.h"
#include "Panels.h"
#include <qlayout.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow( 0, 0, Qt::WDestructiveClose )
{
    setCaption( APP_NAME );
    setFixedSize( 600, 400 );
    const QRect& screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - QPoint( width() / 2, height() / 2 ) );

    QGridLayout* grid = new QGridLayout( this, 1, 2 );
    grid->setColStretch( 0, 1 );
    grid->setColStretch( 1, 3 );
    
    QWidget* list   = new ExerciseList( this );
    QWidget* panels = new Panels( this );
    grid->addWidget( list  , 0, 0 );
    grid->addWidget( panels, 0, 1 );
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // NOTHING
}
