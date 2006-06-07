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

#include <direct.h>
#include <cstdio>
#include <qsplashscreen.h>

App* App::pInstance_ = 0;

//-----------------------------------------------------------------------------
// Name: App constructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (Added support for cmd line specified initial directory)
//-----------------------------------------------------------------------------
App::App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , pSplashScreen_( 0 )
    , b3dEnabled_   ( true )
    , nTimeSeconds_ ( 0 )
    , bODBEditor_   ( false )
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

    SetSplashText( tr("Démarrage...") );
    pMOSServer_      = new Network    ();
    pAgentManager_   = new AgentManager ();
    pLineManager_    = new LineManager  ();
    pObjectManager_  = new ObjectManager();
    pWeatherManager_ = new Meteo_Manager();

    SetSplashText( tr("Chargement des données...") );

    MT_CommandLine cmdLine( argc(), argv() );
    strRootConfigFile_ = cmdLine.GetOptionStrValue( "-conffile", "./scipio.xml" );
    MT_ExtractFilePath( strRootConfigFile_, strRootDirectory_ );
    InitializeData( strRootConfigFile_ );

    if( cmdLine.IsOptionSet( "-odb" ) )
    {
        SetSplashText( tr("Lecture de l'ODB...") );
        bODBEditor_ = true;
        ReadODB( cmdLine.GetOptionStrValue( "-odb", strODBFilename_ ) );
    }

    // to disable 3d mode
    b3dEnabled_ = ! cmdLine.IsOptionSet( "-no3d" );

    SetSplashText( tr( "Chargement des préférences..." ) );
    pOptions_ = new Options();
    pOptions_->Read();

    SetSplashText( tr( "Initialisation de l'interface..." ) );
    pMainWindow_ = new MainWindow();
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

    //remember the current tick duration
    connect( this, SIGNAL( SpeedChanged( int ) ),  this, SLOT( OnSpeedChanged( int ) ) );
}


//-----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (TacticalLines.xml is output in -conffile directory)
//-----------------------------------------------------------------------------
App::~App()
{
    assert( pInstance_ != 0 );
    pInstance_ = 0;

    // Delete these here so that they wont trigger events while we show a dialog box like the one below.
    delete pNetworkTimer_;
    delete pDisplayTimer_;

    // Save the tactical lines to file.
    if( pLineManager_->NeedSaving() )
    {
        bool saveFile = true;
        if( pOptions_->bAskForTacticalLineSavingOnExit_ )
            saveFile = QMessageBox::question( 0, tr( "TacticalLines.xml" ), 
                                                 tr( "Les limites/limas ont été modifiées:\n voulez-vous sauvegarder TacticalLines.xml ?" ),
                                                 QMessageBox::Yes,
                                                 QMessageBox::No ) == QMessageBox::Yes;
        if( saveFile )
        {
            const std::string strInitialDir = MT_GetCurrentDir();
            MT_ChangeDir( strRootDirectory_ );
            MT_XXmlOutputArchive archive;
            pLineManager_->Write( archive );
            while( ! archive.WriteToFile( "TacticalLines.xml" ) )
            {
                int nResult = QMessageBox::warning( 0, 
                    tr("Erreur d'écriture fichier"),
                    tr("Le fichier de description des limites et limas TacticalLines.xml n'a pas pu etre sauvegardé.\n"
                    "Vérifiez qu'il ne soit pas portegé en écriture"),
                    QMessageBox::Retry,
                    QMessageBox::Abort );
                if( nResult == QMessageBox::Abort )
                    break;
            }
            MT_ChangeDir( strInitialDir );
        }
    }
    
    delete pOptions_;

}


//-----------------------------------------------------------------------------
// Name: App::InitializeData
// Created: NLD 2002-11-28
// Last modified: JVT 03-12-11
//-----------------------------------------------------------------------------
void App::InitializeData( const std::string& strFilename )
{
    const std::string   strCurrentDir = MT_GetCurrentDir();

    std::string         strDir;
    std::string         strFile;
    MT_ExtractFilePath( strFilename, strDir  );
    MT_ExtractFileName( strFilename, strFile );

    MT_ChangeDir      ( strDir );

    InputArchive scipioArchive;
    try
    {
        scipioArchive.Open( strFile );
        scipioArchive.Section( "Scipio" );
        scipioArchive.Section( "Donnees" );

        InitializeTerrainData  ( scipioArchive );
        InitializeRawVisionData( scipioArchive );   
        InitializeMeteo        ( scipioArchive ); 
        InitializeRessourceIDs ( scipioArchive );
        InitializeEquipementIDs( scipioArchive );
        InitializeAgentNBCIDs  ( scipioArchive );
        InitializeSensors      ( scipioArchive );
        InitializeHumanFactors ( scipioArchive ); 
        InitializeObjectIds    ( scipioArchive );

        scipioArchive.ReadField( "ODB", strODBFilename_ );

        scipioArchive.EndSection(); // Donnees
        scipioArchive.EndSection(); // Scipio

        pAgentManager_->Initialize();
    }
    catch( std::exception& e )
    {
        std::stringstream strMsg;
        strMsg << tr("L'un des fichiers SIM contient des erreurs, ASTEC ne peut se lancer.") << std::endl;
        strMsg << tr("Descriptif de l'erreur:") << std::endl << e.what();

        QMessageBox::warning( 0, APP_NAME, strMsg.str().c_str() );
        quit();
        throw;
    }

    // Limas and limits (if the file exists)
    MT_XXmlInputArchive archive;
    if( archive.Open( "TacticalLines.xml" ) )
    {
        archive.EnableExceptions( true );
        try
        {
            pLineManager_->Read( archive );
        }
        catch( std::exception& e )
        {
            std::stringstream strMsg;
            strMsg << tr("Le fichier de description des limites et limas TacticalLines.xml contient des erreurs, et n'a été que partiellement chargé.") << std::endl;
            strMsg << tr("Descriptif de l'erreur:") << std::endl << e.what();

            QMessageBox::warning( 0, APP_NAME, strMsg.str().c_str() );
        }
    }

    MT_ChangeDir( strCurrentDir );
}


// -----------------------------------------------------------------------------
// Name: App::InitializeRawVisionData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeRawVisionData( InputArchive& scipioArchive )
{
    std::string strTerFile;
    scipioArchive.ReadField( "Terrain", strTerFile );

    std::string strTerrainDataPath;
    MT_ExtractFilePath( strTerFile, strTerrainDataPath );

    InputArchive terrainArchive;
    terrainArchive.Open( strTerFile );
    terrainArchive.Section( "Terrain" );

    std::string strRawVisionDirectory;
    terrainArchive.ReadField( "RawVision", strRawVisionDirectory );
    rawData_.InitializeHeight( strTerrainDataPath + strRawVisionDirectory + "/detection.dat" );

    terrainArchive.EndSection(); // Terrain
    terrainArchive.Close();
}


// -----------------------------------------------------------------------------
// Name: App::InitializeMeteo
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeMeteo( InputArchive& scipioArchive )
{
    // Meteo
    std::string strMeteoFile;
    scipioArchive.ReadField( "Meteo", strMeteoFile );
    rawData_.InitializeMeteo( strMeteoFile );
}


// -----------------------------------------------------------------------------
// Name: App::InitializeTerrainData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeTerrainData( InputArchive& scipioArchive )
{
    std::string strTerFile;
    scipioArchive.ReadField( "Terrain", strTerFile );
    pWorld_ = new World( strTerFile );
}

// -----------------------------------------------------------------------------
// Name: App::InitializeRessourceIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeRessourceIDs( InputArchive& scipioArchive )
{
    std::string strDotationFile;
    scipioArchive.ReadField( "Dotations", strDotationFile );

    InputArchive dotArchive;
    dotArchive.Open( strDotationFile );
    dotArchive.BeginList( "Dotations" );

    while( dotArchive.NextListElement() )
    {
        dotArchive.Section( "Dotation" );
        std::string strDot;
        dotArchive.ReadAttribute( "nom", strDot );
        dotArchive.BeginList( "Categories" );
        while( dotArchive.NextListElement() )
        {
            dotArchive.Section( "Categorie" );
            std::string strName;
            uint        nID;
            dotArchive.ReadAttribute( "nom", strName );
            dotArchive.ReadField( "MosID", nID );

            resourcesNameMap_.insert( std::make_pair( nID, strName ) );
            resourcesMap_.insert( std::make_pair( nID, Resource( nID, strName, strDot ) ) );

            dotArchive.EndSection(); // Categorie
        }
        dotArchive.EndList(); // Categories
        dotArchive.EndSection(); // Dotations
    }
    dotArchive.EndList(); // Dotations

    dotArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: App::InitializeEquipementIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeEquipementIDs( InputArchive& scipioArchive )
{
    std::string strCompFile;
    scipioArchive.ReadField( "Composantes", strCompFile );

    InputArchive compArchive;
    compArchive.Open( strCompFile );

    compArchive.BeginList( "Composantes" );

    while( compArchive.NextListElement() )
    {
        compArchive.Section( "Composante" );

        std::string strName;
        uint        nID;
        compArchive.ReadAttribute( "nom", strName );
        compArchive.ReadField( "MosID", nID );

        equipementNameMap_.insert( std::make_pair( nID, strName ) );

        compArchive.EndSection(); // Composante
    }
    compArchive.EndList(); // Composantes
    compArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: App::InitializeAgentNBCIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void App::InitializeAgentNBCIDs( InputArchive& scipioArchive )
{
    std::string strNBCFile;
    scipioArchive.ReadField( "NBC", strNBCFile );

    InputArchive nbcArchive;
    nbcArchive.Open( strNBCFile );
    nbcArchive.Section( "NBC" );

    nbcArchive.BeginList( "AgentsNBC" );
    while( nbcArchive.NextListElement() )
    {
        nbcArchive.Section( "AgentNBC" );

        std::string strName;
        uint        nID;
        nbcArchive.ReadAttribute( "nom", strName );
        nbcArchive.ReadField( "MosID", nID );

        nbcNamesMap_.insert( std::make_pair( nID, strName ) );

        nbcArchive.EndSection(); // AgentNBC
    }
    nbcArchive.EndList(); // AgentsNBC
    nbcArchive.EndSection(); // NBC
    nbcArchive.Close();
}


// -----------------------------------------------------------------------------
// Name: App::InitializeSensors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void App::InitializeSensors( InputArchive& scipioArchive )
{
    std::string strSensorFile;
    scipioArchive.ReadField( "Capteurs", strSensorFile );

    InputArchive sensorArchive;
    sensorArchive.Open( strSensorFile );
    sensorArchive.Section( "Capteurs" );

    sensorArchive.BeginList( "Senseurs" );
    while( sensorArchive.NextListElement() )
    {
        sensorArchive.Section( "Senseur" );

        std::string strName;
        sensorArchive.ReadAttribute( "nom", strName );

        if( sensorArchive.Section( "DetectionAgents", InputArchive::eNothing ) )
        {
            SensorType& sensor = *new SensorType( strName, sensorArchive );
            const SensorType*& pSensor = sensorTypes_[ sensor.GetName() ];
            assert( !pSensor );
            pSensor = &sensor; 
            sensorArchive.EndSection();
        }
        sensorArchive.EndSection(); // Senseur
    }

    sensorArchive.EndList(); // Senseurs
    sensorArchive.EndSection(); // Capteurs
    sensorArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: App::InitializeHumanFactors
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void App::InitializeHumanFactors( InputArchive& scipioArchive )
{
    std::string strHumanFactorsFile;
    scipioArchive.ReadField( "FacteursHumains", strHumanFactorsFile );

    InputArchive humanFactorsArchive;
    humanFactorsArchive.Open( strHumanFactorsFile );

    Tiredness ::Initialize( humanFactorsArchive );
    Experience::Initialize( humanFactorsArchive );
    Morale    ::Initialize();

    humanFactorsArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: App::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void App::InitializeObjectIds( InputArchive& scipioArchive )
{
    std::string strClassId;
    scipioArchive.ReadField( "ClasseIDs", strClassId );
    InputArchive classeIds;
    classeIds.Open( strClassId );
    Object_ABC::InitializeObjectIds( classeIds );
}

// -----------------------------------------------------------------------------
// Name: App::ReadODB
/** @param  strFilename 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void App::ReadODB( std::string strFilename )
{
    const std::string   strCurrentDir = MT_GetCurrentDir();

    std::string         strDir;
    std::string         strFile;
    MT_ExtractFilePath( strFilename, strDir  );
    MT_ExtractFileName( strFilename, strFile );

    MT_ChangeDir      ( strDir );

    InputArchive archive;
    if( ! archive.Open( strFile, InputArchive::eNothing ) )
    {
        strODBFilename_ = "";
        return;
    }

    strODBFilename_ = strFilename;
    try
    {
        archive.Section( "ODB" );
        pAgentManager_->ReadODB( archive );
        pObjectManager_->ReadODB( archive );
        archive.EndSection();
    }
    catch( std::exception& e )
    {
        // Clean up what has been partially read.
        pAgentManager_->DeleteAllAgents();
        pAgentManager_->DeleteAllGtias();
        pObjectManager_->DeleteAllObjects();

        std::stringstream strMsg;
        strMsg << tr("Le fichier ODB ") << strFilename << tr(" contient des erreurs, et n'a pu être chargé.") << std::endl;
        strMsg << tr("Descriptif de l'erreur:") << std::endl << e.what();
        QMessageBox::critical( 0, APP_NAME, strMsg.str().c_str() );
    }

    MT_ChangeDir( strCurrentDir );
}


// -----------------------------------------------------------------------------
// Name: App::WriteODB
/** @param  strFilename 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void App::WriteODB( std::string strFilename )
{
    strODBFilename_ = strFilename;

    MT_XXmlOutputArchive archive;

    archive.Section( "ODB" );
    pAgentManager_->WriteODB( archive );
    pObjectManager_->WriteODB( archive );
    archive.EndSection();

    while( ! archive.WriteToFile( strFilename ) )
    {
        std::stringstream strMsg;
        strMsg << tr("Le fichier ODB ") << strFilename << tr(" n'a pas pu etre sauvegardé.") << std::endl;
        strMsg << tr("Vérifiez qu'il ne soit pas portégé en écriture");
        int nResult = QMessageBox::warning( 0, 
            tr("Erreur d'écriture fichier"),
            strMsg.str().c_str(),
            QMessageBox::Retry,
            QMessageBox::Abort );
        if( nResult == QMessageBox::Abort )
            break;
    }
}


// -----------------------------------------------------------------------------
// Name: App::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void App::WriteODB()
{
    if( ! strODBFilename_.empty() )
        this->WriteODB( strODBFilename_ );
    else
    {
        QString strFilename = QFileDialog::getSaveFileName( QString::null, "*.xml", 0 );
        if( strFilename == QString::null )
            return;
        this->WriteODB( strFilename.ascii() );
    }
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

//=============================================================================
// ACTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: App::NotifyTimeChanged
// Created: NLD 2002-09-16
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void App::NotifyTimeChanged( uint nTimeInSeconds )
{
    nTimeSeconds_ = nTimeInSeconds;
    rawData_.UpdateEphemeride();
    emit TimeChanged( nTimeInSeconds );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyTickStartEnd
/** @param  bTickStart 
*/
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void App::NotifyTickStartEnd( bool bTickStart, uint tick )
{
    emit TickStartEnd( bTickStart, tick );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyConnexionLost
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void App::NotifyConnexionLost()
{
    NotifyConnexionStatusChanged( false );
    GetAgentManager().DeleteAllConflicts();
    GetAgentManager().DeleteAllAgents();
    GetAgentManager().DeleteAllTeams();
    GetLineManager ().OnDeconnexion();
    GetObjectManager().DeleteAllObjects();
}

// -----------------------------------------------------------------------------
// Name: App::NotifyConnexionStatusChanged
/** @param  bConnected 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void App::NotifyConnexionStatusChanged( bool bConnected )
{
    emit ConnexionStatusChanged( bConnected );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyPauseStatusChanged
/** @param  bPaused 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void App::NotifyPauseStatusChanged( bool bPaused )
{
    emit PauseStatusChanged( bPaused );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentCreated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void App::NotifyAgentCreated( Agent& agent )
{
    emit AgentCreated( agent );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void App::NotifyAgentUpdated( Agent& agent )
{
    emit AgentUpdated( agent );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentReparented
/** @param  agent 
*/
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void App::NotifyAgentReparented( Agent& agent )
{
    emit AgentReparented( agent );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationCreated
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void App::NotifyPopulationCreated( Population& population )
{
    emit PopulationCreated( population );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationUpdated
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void App::NotifyPopulationUpdated( Population& population )
{
	emit PopulationUpdated( population );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void App::NotifyLogisticConsignUpdated( LogSupplyConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetAutomateLogHandling() )
        emit AgentUpdated( *consign.GetAutomateLogHandling() );
    if( consign.GetPionLogConvoying() )
        emit AgentUpdated( *consign.GetPionLogConvoying() );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void App::NotifyLogisticConsignUpdated( LogMedicalConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetPionLogHandling() )
        emit AgentUpdated( *consign.GetPionLogHandling() );
}
    
// -----------------------------------------------------------------------------
// Name: App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void App::NotifyLogisticConsignUpdated( LogMaintenanceConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetPionLogHandling() )
        emit AgentUpdated( *consign.GetPionLogHandling() );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyObjectCreated
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void App::NotifyObjectCreated( Object_ABC& object )
{
    emit ObjectCreated( object );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyObjectUpdated
/** @param  object 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void App::NotifyObjectUpdated( Object_ABC& object )
{
    emit ObjectUpdated( object );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyObjectDeleted
/** @param  object 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void App::NotifyObjectDeleted( Object_ABC& object )
{
    emit ObjectDeleted( object );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyGtiaCreated
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void App::NotifyGtiaCreated( Gtia& gtia )
{
    emit GtiaCreated( gtia );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyTeamCreated
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void App::NotifyTeamCreated( Team& team )
{
    emit TeamCreated( team );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyTeamDeleted
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void App::NotifyTeamDeleted( Team& team )
{
    emit TeamDeleted( team );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyGtiaDeleted
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void App::NotifyGtiaDeleted( Gtia& gtia )
{
    emit GtiaDeleted( gtia );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyReportCreated
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void App::NotifyReportCreated( Agent_ABC& agent, Report_ABC& report )
{
    emit ReportCreated( agent, report );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentOutOfGas
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void App::NotifyAgentOutOfGas( Agent& agent )
{
    emit AgentOutOfGas( agent );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentRefueled
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void App::NotifyAgentRefueled( Agent& agent )
{
    emit AgentRefueled( agent );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyConflictStarted
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
void App::NotifyConflictStarted( Agent_ABC& origin )
{
    emit ConflictStarted( origin );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyConflictEnded
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
void App::NotifyConflictEnded( Agent_ABC& origin )
{
    emit ConflictEnded( origin );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyObjectExplosion
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void App::NotifyObjectExplosion( Object_ABC& object )
{
    emit ObjectExplosion( object );
}


// -----------------------------------------------------------------------------
// Name: App::NotifySpeedChanged
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void App::NotifySpeedChanged( int nSpeed )
{
    emit SpeedChanged( nSpeed );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentKnowledgeCreated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyAgentKnowledgeCreated( Gtia& gtia, AgentKnowledge& knowledge )
{
    emit AgentKnowledgeCreated( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentKnowledgeUpdated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyAgentKnowledgeUpdated( Gtia& gtia, AgentKnowledge& knowledge )
{
    emit AgentKnowledgeUpdated( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyAgentKnowledgeDeleted
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyAgentKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge )
{
    emit AgentKnowledgeDeleted( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyObjectKnowledgeCreated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyObjectKnowledgeCreated( Team& team, ObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeCreated( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyObjectKnowledgeUpdated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyObjectKnowledgeUpdated( Team& team, ObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeUpdated( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyObjectKnowledgeDeleted
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void App::NotifyObjectKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeDeleted( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: App::NotifyTacticalLineDeleted
/** @param  line 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void App::NotifyTacticalLineDeleted( TacticalLine_ABC& line )
{
    emit TacticalLineDeleted( line );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyTypePopulationCreated
// Created: HME 2005-10-19
// -----------------------------------------------------------------------------
void App::NotifyTypePopulationCreated ( TypePopulation& type )
{
    emit TypePopulationCreated( type );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationCreated( const PopulationConcentration& concentration )
{
    emit PopulationConcentrationCreated( concentration ); 
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationUpdated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationUpdated( const PopulationConcentration& concentration )
{
    emit PopulationConcentrationUpdated( concentration ); 
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationDeleted
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationDeleted( const PopulationConcentration& concentration )
{
    emit PopulationConcentrationDeleted( concentration );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowCreated
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowCreated( const PopulationFlow& flow )
{
    emit PopulationFlowCreated( flow );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowUpdated
// Updated: SBO 2006-01-24
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowUpdated( const PopulationFlow& flow )
{
    emit PopulationFlowUpdated( flow );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowDeleted
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowDeleted( const PopulationFlow& flow )
{
    emit PopulationFlowDeleted( flow );
}

// -----------------------------------------------------------------------------
// Name: App::SetSplashText
/** @param  strText 
*/
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void App::SetSplashText( const QString& strText )
{
    if( pSplashScreen_ != 0 )
        pSplashScreen_->message( strText, Qt::AlignBottom | Qt::AlignHCenter, Qt::white );
}


//-----------------------------------------------------------------------------
// Name: App::GetResourceName
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string App::GetResourceName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = resourcesNameMap_.find( id );
    if( found != resourcesNameMap_.end() )
        return found->second;
    else
        return itostring( id );
}


//-----------------------------------------------------------------------------
// Name: App::GetEquipmentName 
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string App::GetEquipmentName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = equipementNameMap_.find( id );
    if( found != equipementNameMap_.end() )
        return found->second;
    else
        return itostring( id );    
}

// -----------------------------------------------------------------------------
// Name: App::GetEquipementID
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
uint App::GetEquipementID( const std::string& strName ) const
{
    for( CIT_MosId_String_Map it = equipementNameMap_.begin(); it != equipementNameMap_.end(); ++it )
        if( it->second == strName )
            return it->first;
    return (unsigned)(-1);
}


//-----------------------------------------------------------------------------
// Name: App::GetBreakDownName
// Created: JVT 04-04-09
//-----------------------------------------------------------------------------
std::string App::GetBreakDownName( uint id ) const
{
    CIT_MosId_String_Map found = breakDownsNameMap_.find( id );
    if( found != breakDownsNameMap_.end() )
        return found->second;
    else
        return itostring( id );    
}

// -----------------------------------------------------------------------------
// Name: App::GetRessourceID
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
unsigned int App::GetRessourceID( const std::string& strRessource ) const
{
    for( CIT_MosId_String_Map it = resourcesNameMap_.begin(); it != resourcesNameMap_.end(); ++it )
        if( it->second == strRessource )
            return it->first;
    return (unsigned)(-1);
}

// -----------------------------------------------------------------------------
// Name: App::Is3D
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
bool App::Is3D() const
{
    return pMainWindow_ && pOptions_->b3D_;
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationKnowledgeCreated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void App::NotifyPopulationKnowledgeCreated( Gtia& gtia, PopulationKnowledge& knowledge )
{
    emit PopulationKnowledgeCreated( gtia, knowledge );
}
    
// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationKnowledgeUpdated
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void App::NotifyPopulationKnowledgeUpdated( Gtia& gtia, PopulationKnowledge& knowledge )
{
    emit PopulationKnowledgeUpdated( gtia, knowledge );
}
    
// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationKnowledgeDeleted
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void App::NotifyPopulationKnowledgeDeleted( Gtia& gtia, PopulationKnowledge& knowledge )
{
    emit PopulationKnowledgeDeleted( gtia, knowledge );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationKnowledgeCreated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationKnowledgeCreated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    emit PopulationConcentrationKnowledgeCreated( gtia, knowledge );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationKnowledgeUpdated
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationKnowledgeUpdated( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    emit PopulationConcentrationKnowledgeUpdated( gtia, knowledge );
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationConcentrationKnowledgeDeleted
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
void App::NotifyPopulationConcentrationKnowledgeDeleted( Gtia& gtia, PopulationConcentrationKnowledge& knowledge )
{
    emit PopulationConcentrationKnowledgeDeleted( gtia, knowledge );
}

// -----------------------------------------------------------------------------
// Name: App::Is3DEnabled
// Created: SBO 2006-01-24
// -----------------------------------------------------------------------------
bool App::Is3DEnabled() const
{
    return b3dEnabled_;
}
// -----------------------------------------------------------------------------
// Name: App::OnSpeedChanged
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
void App::OnSpeedChanged( int nTickDuration )
{
    nTickDuration_ = nTickDuration;
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowKnowledgeCreated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowKnowledgeCreated( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    emit PopulationFlowKnowledgeCreated( gtia, knowledge );
}
    
// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowKnowledgeUpdated
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowKnowledgeUpdated( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    emit PopulationFlowKnowledgeUpdated( gtia, knowledge );
}
    
// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationFlowKnowledgeDeleted
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void App::NotifyPopulationFlowKnowledgeDeleted( Gtia& gtia, PopulationFlowKnowledge& knowledge )
{
    emit PopulationFlowKnowledgeDeleted( gtia, knowledge );
}

// -----------------------------------------------------------------------------
// Name: App::GetTickDuration
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
uint App::GetTickDuration() const
{
    return nTickDuration_;
}

// -----------------------------------------------------------------------------
// Name: App::NotifyPopulationDeleted
// Created: HME 2006-01-03
// -----------------------------------------------------------------------------
void App::NotifyPopulationDeleted( Population& population )
{
    emit PopulationDeleted( population );
}

// -----------------------------------------------------------------------------
// Name: App::GetOptions
// Created: SBO 2006-04-11
// -----------------------------------------------------------------------------
Options& App::GetOptions() const
{
    return *pOptions_;
}
