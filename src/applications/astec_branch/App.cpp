//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/App.cpp $
// $Author: Age $
// $Modtime: 16/05/05 17:00 $
// $Revision: 25 $
// $Workfile: App.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "App.h"
#include "moc_App.cpp"

#include "Network.h"
#include "MainWindow.h"
#include "Morale.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Options.h"
#include "Model.h"
#include "Simulation.h"
#include "Controllers.h"
#include "Workers.h"
#include "InitializationMessage.h"
#include "AgentServerMsgMgr.h"

#include "xeumeuleu/xml.h"

#include <qsplashscreen.h>
#include <qfileinfo.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

using namespace xml;
namespace po = boost::program_options;
namespace bfs = boost::filesystem;

App* App::pInstance_ = 0;

namespace 
{
    struct CatchMeIfYouCan {};
}


//-----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
App::App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , mainWindow_( 0 )
{
    try
    {
        Initialize( nArgc, ppArgv );
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
// Name: App::Initialize
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void App::Initialize( int nArgc, char** ppArgv )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;

    // Command line options
    std::string conffile;
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "conffile,c", po::value< std::string >( &conffile )->default_value( "./scipio.xml" ), "specify main config file (scipio.xml)" )
    ;
    po::variables_map vm;
    po::store( po::parse_command_line( nArgc, ppArgv, desc ), vm );
    po::notify( vm );

    controllers_ = new Controllers();
    simulation_  = new Simulation( *controllers_ );
    workers_     = new Workers();
    model_       = new Model( *controllers_, *simulation_, *workers_ );
    network_     = new Network( *controllers_, *model_, *simulation_ );
    mainWindow_  = new MainWindow( *controllers_, *model_, network_->GetMessageMgr().GetMsgRecorder() ); // $$$$ AGE 2006-05-03: 

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
// Name: App::BuildChildPath
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
std::string App::BuildChildPath( const std::string& parent, const std::string& child )
{
    bfs::path parentPath( parent.c_str(), bfs::native );
    bfs::path childPath( child.c_str(), bfs::native );

    return ( parentPath.branch_path() / childPath ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: App::BuildWorkingDirectoryPath
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
std::string App::BuildWorkingDirectoryPath( const std::string& file )
{
    bfs::path cwd( ".", bfs::native );
    return ( cwd.branch_path() / file ).native_file_string();
}

//-----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
App::~App()
{
    // bof
}

//-----------------------------------------------------------------------------
// Name: App::UpdateData
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void App::UpdateData()
{
    static bool bDoMe = true;
    try
    {
        if( bDoMe )
            network_->Update();
    } catch( std::exception& e )
    {
        network_->Disconnect();
        // $$$$ AGE 2006-05-11: Huhu
        QMessageBox::critical( 0, APP_NAME, e.what(), QMessageBox::Abort, QMessageBox::Abort );
    }
}

// -----------------------------------------------------------------------------
// Name: App::GetApp
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
App& App::GetApp()
{
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: App::GetNetwork
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
Network& App::GetNetwork() const
{
    return *network_;
}

