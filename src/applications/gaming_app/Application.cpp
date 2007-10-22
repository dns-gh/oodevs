// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Application.h"
#include "moc_Application.cpp"

#include "MainWindow.h"
#include "Config.h"
#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Model.h"
#include "gaming/Simulation.h"
#include "gaming/Profile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Workers.h"
#include "gaming/AgentServerMsgMgr.h"
#include "RcEntityResolver.h"

using namespace kernel;

namespace 
{
    struct CatchMeIfYouCan {};
}

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication( argc, argv )
    , mainWindow_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::~Application()
{
    delete network_;
    delete workers_;
    delete config_;
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::Initialize()
{
    try
    {
        Initialize( argc(), argv() );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, APP_NAME, e.what() );
        throw CatchMeIfYouCan();
    }
    catch( ... )
    {
        QMessageBox::critical( 0, APP_NAME, "Whooops. CSword has crashed" );
        throw CatchMeIfYouCan();
    }
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::Initialize( int argc, char** argv )
{
    config_      = new Config( argc, argv );
    controllers_ = new Controllers();
    simulation_  = new Simulation( controllers_->controller_  );
    profile_     = new Profile( *controllers_ );
    workers_     = new Workers();
    network_     = new Network( *simulation_, *profile_ );
    RcEntityResolver_ABC* rcResolver = new RcEntityResolver( this, *controllers_ );
    staticModel_ = new StaticModel( *controllers_, *rcResolver, *simulation_ );
    model_       = new Model( *controllers_, *staticModel_, *simulation_, *workers_, network_->GetMessageMgr(), *rcResolver );
    network_->GetMessageMgr().SetModel( *model_ );
    mainWindow_  = new MainWindow( *controllers_, *staticModel_, *model_, *simulation_, *network_, *profile_, *config_ );
    mainWindow_->show();

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );

    // The following 2 timers will roughly emulate two threads.
    // This one reads the network to refresh the local data and is called as often as possible.
    networkTimer_ = new QTimer( this );
    connect( networkTimer_, SIGNAL( timeout()), SLOT( UpdateData() ) );
    networkTimer_->start(0);

    config_->Connect( *network_ );
}

// -----------------------------------------------------------------------------
// Name: Application::UpdateData
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::UpdateData()
{
    try
    {
        workers_->CommitTasks();
        network_->Update();
    } 
    catch( std::exception& e )
    {
        network_->Disconnect();
        QMessageBox::critical( 0, APP_NAME, e.what(), QMessageBox::Abort, QMessageBox::Abort );
    }
}
