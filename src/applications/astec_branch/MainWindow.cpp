// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MainWindow.cpp $
// $Author: Age $
// $Modtime: 17/05/05 14:05 $
// $Revision: 20 $
// $Workfile: MainWindow.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "Logger.h"
#include "Options.h"

MainWindow* MainWindow::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::MainWindow()
    : QMainWindow  ( 0, 0, Qt::WDestructiveClose )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    this->setIcon( MAKE_PIXMAP( mosicon ) );
    this->setCaption( APP_NAME );

    pOptions_ = new Options();

    setCentralWidget( new Logger( this ) );
}


// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    assert( pInstance_ != 0 );

    delete pOptions_;

    pInstance_ = 0;
}


// -----------------------------------------------------------------------------
// Name: MainWindow::Update
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MainWindow::Update()
{
//    pGLWidget_->update();
}


