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
#include "AgentManager.h"
#include "LineManager.h"
#include "ObjectManager.h"
#include "Meteo_Manager.h"
#include "MainWindow.h"
#include "SensorType.h"
#include "Morale.h"
#include "Experience.h"
#include "Tiredness.h"
#include "LogSupplyConsign.h"
#include "LogMedicalConsign.h"
#include "LogMaintenanceConsign.h"
#include "Object_ABC.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "Options.h"
#include "Model.h"
#include "Simulation.h"
#include "AgentFactory.h"
#include "ObjectFactory.h"
#include "AgentTypes.h"
#include "Controller.h"

#include "xeumeuleu/xml.h"

#include <direct.h>
#include <cstdio>
#include <qsplashscreen.h>
#include <qfileinfo.h>

#include <boost/program_options.hpp>

using namespace xml;
namespace po = boost::program_options;

App* App::pInstance_ = 0;

//-----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (Added support for cmd line specified initial directory)
//-----------------------------------------------------------------------------
App::App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , pSplashScreen_( 0 )
    , pMainWindow_( 0 )
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
        pSplashScreen_ = new QSplashScreen( splashImage );
        pSplashScreen_->show();
    }

//    SetSplashText( tr("Démarrage...") );

    conffile = RetrieveValidConfigFile( conffile );
    Initialize( conffile );

//    SetSplashText( tr("Initialisation de l'interface...") );
    pMainWindow_ = new MainWindow( *controller_, conffile );
    pMainWindow_->show();

    if( pSplashScreen_ )
    {
        pSplashScreen_->finish( pMainWindow_ );
        delete pSplashScreen_;
        pSplashScreen_ = 0;
    }

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

    // The following 2 timers will roughly emulate two threads.
    // This one reads the network to refresh the local data and is called as often as possible.
    pNetworkTimer_ = new QTimer( this );
    connect( pNetworkTimer_, SIGNAL( timeout()), this, SLOT( UpdateData() ) );
    pNetworkTimer_->start(0);

    // This one refreshes the map display, and is called only a few time per second.
    pDisplayTimer_ = new QTimer( this );
    connect( pDisplayTimer_, SIGNAL( timeout()), this, SLOT( UpdateDisplay() ) );
    pDisplayTimer_->start(250);
}

// -----------------------------------------------------------------------------
// Name: App::Initialize
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void App::Initialize( const std::string& scipioXml )
{
    const QString initialDirectory = QDir::currentDirPath();

    xifstream xis( scipioXml );
    QDir::setCurrent( QFileInfo( scipioXml.c_str() ).dirPath() );

    xis >> start( "Scipio" )
            >> start( "Donnees" );
//    InitializeTerrainData  ( xis );
    InitializeHumanFactors ( xis ); 

    controller_      = new Controller();
    simulation_      = new Simulation();
    model_           = new Model( *controller_, *simulation_, scipioXml );
    pMOSServer_      = new Network( *model_, *simulation_ );

    QDir::setCurrent( initialDirectory );

    pMOSServer_->Connect( "localhost", 10000 );
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
void App::InitializeHumanFactors( xistream& xis )
{
    std::string strHumanFactorsFile;
    xis >> content( "FacteursHumains", strHumanFactorsFile );
    xifstream factors( strHumanFactorsFile );

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
    assert( pMOSServer_ != 0 );
    pMOSServer_->Update();
    pMOSServer_->Update();
    pMOSServer_->Update();
}
    

//-----------------------------------------------------------------------------
// Name: App::UpdateDisplay
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void App::UpdateDisplay()
{
    assert( pMainWindow_ != 0 );
    pMainWindow_->Update();
}