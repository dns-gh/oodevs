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
#include "Controller.h"

#include "xeumeuleu/xml.h"

#include <qsplashscreen.h>
#include <qfileinfo.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#pragma warning( pop )

using namespace xml;
namespace po = boost::program_options;

App* App::pInstance_ = 0;

//-----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
App::App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , splashScreen_( 0 )
    , mainWindow_( 0 )
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
    po::store( po::parse_command_line( argc(), argv(), desc ), vm );
    po::notify( vm );
    
    // Prepare the splash screen displayed during the initialization.
    QPixmap splashImage( "Mos2.jpg" );
    if( !splashImage.isNull() )
    {
        splashScreen_ = new QSplashScreen( splashImage );
        splashScreen_->show();
    }

//    SetSplashText( tr("Démarrage...") );

    conffile = RetrieveValidConfigFile( conffile );
    Initialize( conffile );

//    SetSplashText( tr("Initialisation de l'interface...") );
    mainWindow_ = new MainWindow( *controller_, conffile );
    mainWindow_->show();

    if( splashScreen_ )
    {
        splashScreen_->finish( mainWindow_ );
        delete splashScreen_;
        splashScreen_ = 0;
    }

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

    // The following 2 timers will roughly emulate two threads.
    // This one reads the network to refresh the local data and is called as often as possible.
    networkTimer_ = new QTimer( this );
    connect( networkTimer_, SIGNAL( timeout()), this, SLOT( UpdateData() ) );
    networkTimer_->start(0);

    // This one refreshes the map display, and is called only a few time per second.
    displayTimer_ = new QTimer( this );
    connect( displayTimer_, SIGNAL( timeout()), this, SLOT( UpdateDisplay() ) );
    displayTimer_->start(250);
}

// -----------------------------------------------------------------------------
// Name: App::Initialize
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void App::Initialize( const std::string& scipioXml )
{
    const std::string absoluteConfigFile = QFileInfo( scipioXml.c_str() ).absFilePath().ascii();

    xifstream xis( absoluteConfigFile );
    xis >> start( "Scipio" )
            >> start( "Donnees" );
    InitializeHumanFactors ( xis, absoluteConfigFile ); 

    controller_ = new Controller();
    simulation_ = new Simulation();
    model_      = new Model( *controller_, *simulation_, absoluteConfigFile );
    network_    = new Network( *model_, *simulation_ );

    network_->Connect( "localhost", 10000 ); // $$$$ SBO 2006-03-16: 
}

//-----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
App::~App()
{
    // bof
}

// -----------------------------------------------------------------------------
// Name: App::RetrieveValidConfigFile
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
std::string App::RetrieveValidConfigFile( const std::string& conffile )
{
    std::string current = conffile;
    while( ! QFileInfo( current.c_str() ).exists() )
    {
        const QString filename = QFileDialog::getOpenFileName( "../data/", "Scipio (*.xml)", 0, 0, "Open scipio.xml" );
        if( ! filename.isEmpty() )
            current = filename;
        else
            throw std::exception( "No scipio.xml file specified." );
    }
    return current;
}

// -----------------------------------------------------------------------------
// Name: App::InitializeHumanFactors
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void App::InitializeHumanFactors( xistream& xis, const std::string& conffile )
{
    const std::string baseDirectory = QFileInfo( conffile.c_str() ).dirPath().ascii() + std::string( "/" );

    std::string strHumanFactorsFile;
    xis >> content( "FacteursHumains", strHumanFactorsFile );
    xifstream factors( baseDirectory + strHumanFactorsFile );

    Tiredness ::Initialize( factors );
    Experience::Initialize( factors );
    Morale    ::Initialize();
}

//-----------------------------------------------------------------------------
// Name: App::UpdateData
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void App::UpdateData()
{
    network_->Update();
    network_->Update();
    network_->Update();
}
    

//-----------------------------------------------------------------------------
// Name: App::UpdateDisplay
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void App::UpdateDisplay()
{
    mainWindow_->Update();
}

