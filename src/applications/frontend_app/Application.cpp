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
#include "moc_Application.cpp"
#include "MainWindow.h"
#include "frontend/Networker.h"
#include <qtimer.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication ( argc, argv )
    , networker_   ( new Networker( "127.0.0.1:10000" ) )
    , networkTimer_( new QTimer( this ) )
    , mainWindow_  ( 0 )
{
    mainWindow_  = new MainWindow();
    mainWindow_->show();

    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
    connect( networkTimer_, SIGNAL( timeout() ), this, SLOT( UpdateData() ) );
    networkTimer_->start(0);
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application::UpdateData
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
void Application::UpdateData()
{
    try
    {
        networker_->Update();
    } 
    catch( std::exception& e )
    {
        networker_->Disconnect();
        QMessageBox::critical( 0, APP_NAME, e.what(), QMessageBox::Abort, QMessageBox::Abort );
    }
}
