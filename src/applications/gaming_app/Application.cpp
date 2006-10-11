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

#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Model.h"
#include "gaming/Simulation.h"
#include "gaming/Profile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Workers.h"
#include "gaming/AgentServerMsgMgr.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

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
    : QApplication  ( argc, argv )
    , mainWindow_( 0 )
{
    try
    {
        Initialize( argc, argv );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, APP_NAME, e.what() );
        throw CatchMeIfYouCan();
    }
    catch( ... )
    {
        QMessageBox::critical( 0, APP_NAME, "Whooops. Astec has crashed" );
        throw CatchMeIfYouCan();
    }
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::Initialize( int argc, char** argv )
{
    // Command line options
    std::string conffile;
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "conffile,c", po::value< std::string >( &conffile )->default_value( "./scipio.xml" ), "specify main config file (scipio.xml)" )
    ;
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    controllers_ = new Controllers();
    simulation_  = new Simulation( controllers_->controller_  );
    profile_     = new Profile( controllers_->controller_ );
    workers_     = new Workers();
    network_     = new Network( *controllers_, *simulation_, *profile_ );
    staticModel_ = new StaticModel( *controllers_ );
    model_       = new Model( *controllers_, *staticModel_, *simulation_, *workers_, network_->GetMessageMgr() );
    network_->GetMessageMgr().SetModel( *model_ );
    mainWindow_  = new MainWindow( *controllers_, *staticModel_, *model_, *network_ );

    if( bfs::exists( bfs::path( conffile, bfs::native ) ) )
        mainWindow_->Load( conffile );

    mainWindow_->show();

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

    // The following 2 timers will roughly emulate two threads.
    // This one reads the network to refresh the local data and is called as often as possible.
    networkTimer_ = new QTimer( this );
    connect( networkTimer_, SIGNAL( timeout()), this, SLOT( UpdateData() ) );
    networkTimer_->start(0);
}

// -----------------------------------------------------------------------------
// Name: Application::UpdateData
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::UpdateData()
{
    try
    {
        network_->Update();
    } 
    catch( std::exception& e )
    {
        network_->Disconnect();
        QMessageBox::critical( 0, APP_NAME, e.what(), QMessageBox::Abort, QMessageBox::Abort );
    }
}
