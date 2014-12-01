// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MainWindow.h"
#include "GLWidget.h"
#include <QtGui/qmessagebox.h>

// -----------------------------------------------------------------------------
// Name: MainWindow constructor
// Created: AGE 2004-12-23
// -----------------------------------------------------------------------------
MainWindow::MainWindow( const std::string& filename )
{
    setMinimumSize( 640, 480 );
    setWindowTitle( "SVG" );

    try
    {
        setCentralWidget( new GLWidget( this, filename ) );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( this, "Blah", e.what(), QMessageBox::Ok, QMessageBox::NoButton );
    }
}

// -----------------------------------------------------------------------------
// Name: MainWindow destructor
// Created: AGE 2004-12-23
// -----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    // NOTHING
}
