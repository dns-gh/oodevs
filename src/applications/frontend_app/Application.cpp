// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Application.h"
#include "MainWindow.h"

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication ( argc, argv )
    , mainWindow_  ( new MainWindow() )
{
    mainWindow_->show();
    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}
