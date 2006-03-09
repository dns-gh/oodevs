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
#include "World.h"
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
#include "MT/MT_IO/MT_CommandLine.h"
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

using namespace xml;

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
    
    // Prepare the splash screen displayed during the initialization.
    QPixmap splashImage( "Mos2.jpg" );
    if( !splashImage.isNull() )
    {
        pSplashScreen_ = new QSplashScreen( splashImage );
        pSplashScreen_->show();
    }

//    SetSplashText( tr("Démarrage...") );

    MT_CommandLine cmdLine( argc(), argv() );
    const std::string conffile = cmdLine.GetOptionStrValue( "-conffile", "./scipio.xml" );
    Initialize( conffile );

//    SetSplashText( tr("Initialisation de l'interface...") );
    pMainWindow_ = new MainWindow( *controller_ );
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
    xifstream xis( scipioXml );

    const std::string currentDirectory = MT_GetCurrentDir();
    std::string baseDirectory;
    MT_ExtractFilePath( scipioXml, baseDirectory );
    MT_ChangeDir( baseDirectory );

    xis >> start( "Scipio" )
            >> start( "Donnees" );
    InitializeTerrainData  ( xis );
    InitializeHumanFactors ( xis ); 

    controller_      = new Controller();
    simulation_      = new Simulation();
    model_           = new Model( *controller_, *simulation_, scipioXml );
    pMOSServer_      = new Network( *model_, *simulation_ );

    MT_ChangeDir( currentDirectory );

    pMOSServer_->Connect( "localhost", 10000 );
};

// -----------------------------------------------------------------------------
// Name: App::InitializeTerrainData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeTerrainData( xistream& xis )
{
    std::string terrainFile, meteoFile;
    xis >> content( "Terrain", terrainFile )
        >> content( "Meteo", meteoFile );
    pWorld_ = new World( terrainFile, meteoFile );
}

//-----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (TacticalLines.xml is output in -conffile directory)
//-----------------------------------------------------------------------------
App::~App()
{
    // bof
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