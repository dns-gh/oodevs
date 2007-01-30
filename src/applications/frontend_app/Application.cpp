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
#include "frontend/Model.h"
#include "frontend/Networker.h"
#include "frontend/Exceptions.h"
#include "frontend/Profile.h"
#include "clients_kernel/Controllers.h"
#include <qtimer.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication ( argc, argv )
    , controllers_ ( new kernel::Controllers() )
    , model_       ( new Model( *controllers_ ) )
    , profile_     ( new Profile( controllers_->controller_ ) )
    , networker_   ( new Networker( "127.0.0.1:10000", controllers_->controller_, *model_, *profile_ ) )
    , networkTimer_( new QTimer( this ) )
    , mainWindow_  ( new MainWindow( *controllers_, *networker_, *profile_ ) )
{
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
    catch( frontend::UserRequestFailed& e )
    {
        QMessageBox::warning( mainWindow_, APP_NAME, e.what(), QMessageBox::Ok, QMessageBox::NoButton );
    }
    catch( std::exception& e )
    {
        networker_->Disconnect();
        QMessageBox::critical( mainWindow_, APP_NAME, e.what(), QMessageBox::Abort, QMessageBox::NoButton );
    }
}
