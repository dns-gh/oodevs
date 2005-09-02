//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_App.cpp $
// $Author: Age $
// $Modtime: 16/05/05 17:00 $
// $Revision: 25 $
// $Workfile: MOS_App.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_App.h"
#include "moc_MOS_App.cpp"

#include "MOS_MOSServer.h"
#include "MOS_AgentManager.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_MainWindow.h"
#include "MOS_SensorType.h"
#include "MOS_Morale.h"
#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_DynaObject_ABC.h"
#include "MOS_Options.h"
#include "MT/MT_IO/MT_CommandLine.h"

#include <direct.h>
#include <cstdio>
#include <qsplashscreen.h>

MOS_App* MOS_App::pInstance_ = 0;

#ifndef MOS_USE_INLINE
#   include "MOS_App.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_App constructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (Added support for cmd line specified initial directory)
//-----------------------------------------------------------------------------
MOS_App::MOS_App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , pSplashScreen_( 0 )
    , nTimeSeconds_ ( 0 )
    , bODBEditor_   ( false )
    , pMainWindow_( 0 )
{
    assert( pInstance_ == 0 );
    pInstance_ = this;
    
    // Prepare the splash screen displayed during the initialization.
    QPixmap splashImage( "Mos2.jpg" );
    pSplashScreen_ = new QSplashScreen( splashImage );
    pSplashScreen_->show();

    this->SetSplashText( tr("Démarrage...") );
    pMOSServer_         = new MOS_MOSServer         ();
    pAgentManager_      = new MOS_AgentManager      ();
    pLineManager_       = new MOS_LineManager       ();
    pDynaObjectManager_ = new MOS_DynaObjectManager ();
    pWeatherManager_    = new MOS_Meteo_Manager     ();

    this->SetSplashText( tr("Chargement des données...") );

    MT_CommandLine cmdLine( argc(), argv() );
    strRootConfigFile_ = cmdLine.GetOptionStrValue( "-conffile", "./scipio.xml" );
    MT_ExtractFilePath( strRootConfigFile_, strRootDirectory_ );
    this->InitializeData( strRootConfigFile_ );

    if( cmdLine.IsOptionSet( "-odb" ) )
    {
        this->SetSplashText( tr("Lecture de l'ODB...") );
        bODBEditor_ = true;
        this->ReadODB( cmdLine.GetOptionStrValue( "-odb", strODBFilename_ ) );
    }

    this->SetSplashText( tr("Initialisation de l'interface...") );
    pMainWindow_ = new MOS_MainWindow();
    pMainWindow_->show();

    pSplashScreen_->finish( pMainWindow_ );
    delete pSplashScreen_;
    pSplashScreen_ = 0;

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


//-----------------------------------------------------------------------------
// Name: MOS_App destructor
// Created: NLD 2002-07-15
// Modified: SBO 2005-07-26 (TacticalLines.xml is output in -conffile directory)
//-----------------------------------------------------------------------------
MOS_App::~MOS_App()
{
    assert( pInstance_ != 0 );
    pInstance_ = 0;

    // Delete these here so that they wont trigger events while we show a dialog box like the one below.
    delete pNetworkTimer_;
    delete pDisplayTimer_;

    // Save the tactical lines to file.
    const std::string strInitialDir = MT_GetCurrentDir();
    MT_ChangeDir( strRootDirectory_ );
    MT_XXmlOutputArchive archive;
    pLineManager_->Write( archive );
    while( true )
    {
        if( archive.WriteToFile( "TacticalLines.xml" ) )
            break;
        else
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
    }
    MT_ChangeDir( strInitialDir );

    delete pDynaObjectManager_;
    delete pAgentManager_;
    delete pLineManager_;
    delete pWorld_;
    delete pMOSServer_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::InitializeData
// Created: NLD 2002-11-28
// Last modified: JVT 03-12-11
//-----------------------------------------------------------------------------
void MOS_App::InitializeData( const std::string& strFilename )
{
    const std::string   strCurrentDir = MT_GetCurrentDir();

    std::string         strDir;
    std::string         strFile;
    MT_ExtractFilePath( strFilename, strDir  );
    MT_ExtractFileName( strFilename, strFile );

    MT_ChangeDir      ( strDir );

    MT_XXmlInputArchive scipioArchive;
    scipioArchive.EnableExceptions( true );

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
    catch( MT_Exception& e )
    {
        std::stringstream strMsg;
        strMsg << tr("L'un des fichiers SIM contient des erreurs, MOSLight 2 ne peut se lancer.") << std::endl;
        strMsg << tr("Descriptif de l'erreur:") << std::endl << e.GetWholeMessage();

        QMessageBox::warning( 0, "MOSLight 2", strMsg.str().c_str() );
        quit();
    }

    // Limas and limits (if the file exists)
    MT_XXmlInputArchive archive;
    bool b = archive.Open( "TacticalLines.xml" );
    if( b )
    {
        try
        {
            archive.EnableExceptions( true );
            pLineManager_->Read( archive );
        }
        catch( MT_Exception& e )
        {
            std::stringstream strMsg;
            strMsg << tr("Le fichier de description des limites et limas TacticalLines.xml contient des erreurs, et n'a été que partiellement chargé.") << std::endl;
            strMsg << tr("Descriptif de l'erreur:") << std::endl << e.GetWholeMessage();

            QMessageBox::warning( 0, "MOSLight 2", strMsg.str().c_str() );
        }
    }

    MT_ChangeDir( strCurrentDir );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeRawVisionData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeRawVisionData( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTerFile;
    scipioArchive.ReadField( "Terrain", strTerFile );

    std::string strTerrainDataPath;
    MT_ExtractFilePath( strTerFile, strTerrainDataPath );

    MT_XXmlInputArchive terrainArchive;
    terrainArchive.EnableExceptions( true );
    terrainArchive.Open( strTerFile );
    terrainArchive.Section( "Terrain" );

    std::string strRawVisionDirectory;
    terrainArchive.ReadField( "RawVision", strRawVisionDirectory );
    rawData_.InitializeHeight( strTerrainDataPath + strRawVisionDirectory + "/detection.dat" );

    terrainArchive.EndSection(); // Terrain
    terrainArchive.Close();
}


// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeMeteo
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeMeteo( MT_InputArchive_ABC& scipioArchive )
{
    // Meteo
    std::string strMeteoFile;
    scipioArchive.ReadField( "Meteo", strMeteoFile );
    rawData_.InitializeMeteo( strMeteoFile );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeTerrainData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeTerrainData( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTerFile;
    scipioArchive.ReadField( "Terrain", strTerFile );
    pWorld_ = new MOS_World( strTerFile );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeRessourceIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeRessourceIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strDotationFile;
    scipioArchive.ReadField( "Dotations", strDotationFile );

    MT_XXmlInputArchive dotArchive;
    dotArchive.EnableExceptions( true );
    dotArchive.Open( strDotationFile );
    dotArchive.BeginList( "Dotations" );

    while( dotArchive.NextListElement() )
    {
        dotArchive.Section( "Dotation" );
        dotArchive.BeginList( "Categories" );
        while( dotArchive.NextListElement() )
        {
            dotArchive.Section( "Categorie" );
            std::string strName;
            uint        nID;
            dotArchive.ReadAttribute( "nom", strName );
            dotArchive.ReadField( "MosID", nID );

            resourcesNameMap_.insert( std::make_pair( nID, strName ) );

            dotArchive.EndSection(); // Categorie
        }
        dotArchive.EndList(); // Categories
        dotArchive.EndSection(); // Dotations
    }
    dotArchive.EndList(); // Dotations

    dotArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeEquipementIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeEquipementIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strCompFile;
    scipioArchive.ReadField( "Composantes", strCompFile );

    MT_XXmlInputArchive compArchive;
    compArchive.EnableExceptions( true );
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
// Name: MOS_App::InitializeAgentNBCIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeAgentNBCIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strNBCFile;
    scipioArchive.ReadField( "NBC", strNBCFile );

    MT_XXmlInputArchive nbcArchive;
    nbcArchive.EnableExceptions( true );
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
// Name: MOS_App::InitializeSensors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void MOS_App::InitializeSensors( MT_InputArchive_ABC& scipioArchive )
{
    std::string strSensorFile;
    scipioArchive.ReadField( "Capteurs", strSensorFile );

    MT_XXmlInputArchive sensorArchive;
    sensorArchive.Open( strSensorFile );
    sensorArchive.Section( "Capteurs" );

    sensorArchive.BeginList( "Senseurs" );
    while( sensorArchive.NextListElement() )
    {
        sensorArchive.Section( "Senseur" );

        std::string strName;
        sensorArchive.ReadAttribute( "nom", strName );

        sensorArchive.EnableExceptions( false );
        if( sensorArchive.Section( "DetectionAgents" ) )
        {
            MOS_SensorType& sensor = *new MOS_SensorType( strName, sensorArchive );
            const MOS_SensorType*& pSensor = sensorTypes_[ sensor.GetName() ];
            assert( !pSensor );
            pSensor = &sensor; 
            sensorArchive.EndSection();
        }
        else
            sensorArchive.ClearErrors();
        sensorArchive.EnableExceptions( true );
        sensorArchive.EndSection(); // Senseur
    }

    sensorArchive.EndList(); // Senseurs
    sensorArchive.EndSection(); // Capteurs
    sensorArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeHumanFactors
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_App::InitializeHumanFactors( MT_InputArchive_ABC& scipioArchive )
{
    std::string strHumanFactorsFile;
    if( !scipioArchive.ReadField( "FacteursHumains", strHumanFactorsFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive humanFactorsArchive;
    if( !humanFactorsArchive.Open( strHumanFactorsFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MOS_Tiredness ::Initialize( humanFactorsArchive );
    MOS_Experience::Initialize( humanFactorsArchive );
    MOS_Morale    ::Initialize();

    humanFactorsArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_App::InitializeObjectIds( MT_InputArchive_ABC& scipioArchive )
{
    std::string strClassId;
    if( !scipioArchive.ReadField( "ClasseIDs", strClassId ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive classeIds;
    classeIds.EnableExceptions( true );
    classeIds.Open( strClassId );
    MOS_DynaObject_ABC::InitializeObjectIds( classeIds );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::ReadODB
/** @param  strFilename 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_App::ReadODB( std::string strFilename )
{
    const std::string   strCurrentDir = MT_GetCurrentDir();

    std::string         strDir;
    std::string         strFile;
    MT_ExtractFilePath( strFilename, strDir  );
    MT_ExtractFileName( strFilename, strFile );

    MT_ChangeDir      ( strDir );

    MT_XXmlInputArchive archive;
    bool b = archive.Open( strFile );
    if( !b )
    {
        strODBFilename_ = "";
        return;
    }

    strODBFilename_ = strFilename;
    archive.EnableExceptions( true );

    try
    {
        archive.Section( "ODB" );
        pAgentManager_->ReadODB( archive );
        pDynaObjectManager_->ReadODB( archive );
        archive.EndSection();
    }
    catch( MT_Exception& e )
    {
        // Clean up what has been partially read.
        pAgentManager_->DeleteAllAgents();
        pAgentManager_->DeleteAllGtias();
        pDynaObjectManager_->DeleteAllDynaObjects();

        std::stringstream strMsg;
        strMsg << tr("Le fichier ODB ") << strFilename << tr(" contient des erreurs, et n'a pu être chargé.") << std::endl;
        strMsg << tr("Descriptif de l'erreur:") << std::endl << e.GetWholeMessage();
        QMessageBox::critical( 0, "MOSLight 2", strMsg.str().c_str() );
        exit();
    }

    MT_ChangeDir( strCurrentDir );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::WriteODB
/** @param  strFilename 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_App::WriteODB( std::string strFilename )
{
    strODBFilename_ = strFilename;

    MT_XXmlOutputArchive archive;

    archive.Section( "ODB" );
    pAgentManager_->WriteODB( archive );
    pDynaObjectManager_->WriteODB( archive );
    archive.EndSection();

    while( true )
    {
        if( archive.WriteToFile( strFilename ) )
            break;
        else
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
}


// -----------------------------------------------------------------------------
// Name: MOS_App::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_App::WriteODB()
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
// Name: MOS_App::UpdateData
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void MOS_App::UpdateData()
{
    assert( pMOSServer_ != 0 );
    pMOSServer_->Update();
    pMOSServer_->Update();
    pMOSServer_->Update();
}
    

//-----------------------------------------------------------------------------
// Name: MOS_App::UpdateDisplay
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void MOS_App::UpdateDisplay()
{
    assert( pMainWindow_ != 0 );
    pMainWindow_->Update();
}

//=============================================================================
// ACTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_App::NotifyTimeChanged
// Created: NLD 2002-09-16
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_App::NotifyTimeChanged( uint nTimeInSeconds )
{
    nTimeSeconds_ = nTimeInSeconds;
    rawData_.UpdateEphemeride();
    emit TimeChanged( nTimeInSeconds );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyTickStartEnd
/** @param  bTickStart 
*/
// Created: APE 2004-06-17
// -----------------------------------------------------------------------------
void MOS_App::NotifyTickStartEnd( bool bTickStart )
{
    emit TickStartEnd( bTickStart );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyConnexionStatusChanged
/** @param  bConnected 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void MOS_App::NotifyConnexionStatusChanged( bool bConnected )
{
    emit ConnexionStatusChanged( bConnected );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyPauseStatusChanged
/** @param  bPaused 
*/
// Created: APE 2004-03-08
// -----------------------------------------------------------------------------
void MOS_App::NotifyPauseStatusChanged( bool bPaused )
{
    emit PauseStatusChanged( bPaused );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentCreated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentCreated( MOS_Agent& agent )
{
    emit AgentCreated( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentUpdated( MOS_Agent& agent )
{
    emit AgentUpdated( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentReparented
/** @param  agent 
*/
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentReparented( MOS_Agent& agent )
{
    emit AgentReparented( agent );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_App::NotifyLogisticConsignUpdated( MOS_LogSupplyConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetPionLogHandling() )
        emit AgentUpdated( *consign.GetPionLogHandling() );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_App::NotifyLogisticConsignUpdated( MOS_LogMedicalConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetPionLogHandling() )
        emit AgentUpdated( *consign.GetPionLogHandling() );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyLogisticConsignUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_App::NotifyLogisticConsignUpdated( MOS_LogMaintenanceConsign& consign )
{
    emit AgentUpdated( consign.GetPion() );
    if( consign.GetPionLogHandling() )
        emit AgentUpdated( *consign.GetPionLogHandling() );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyDynaObjectCreated
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_App::NotifyDynaObjectCreated( MOS_DynaObject_ABC& object )
{
    emit DynaObjectCreated( object );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyDynaObjectUpdated
/** @param  object 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_App::NotifyDynaObjectUpdated( MOS_DynaObject_ABC& object )
{
    emit DynaObjectUpdated( object );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyDynaObjectDeleted
/** @param  object 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_App::NotifyDynaObjectDeleted( MOS_DynaObject_ABC& object )
{
    emit DynaObjectDeleted( object );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyGtiaCreated
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void MOS_App::NotifyGtiaCreated( MOS_Gtia& gtia )
{
    emit GtiaCreated( gtia );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyTeamCreated
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void MOS_App::NotifyTeamCreated( MOS_Team& team )
{
    emit TeamCreated( team );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyGtiaDeleted
/** @param  gtia 
*/
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void MOS_App::NotifyGtiaDeleted( MOS_Gtia& gtia )
{
    emit GtiaDeleted( gtia );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyReportCreated
/** @param  agent 
    @param  rc 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_App::NotifyReportCreated( MOS_Agent& agent, MOS_Report_ABC& report )
{
    emit ReportCreated( agent, report );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentOutOfGas
/** @param  agent 
*/
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentOutOfGas( MOS_Agent& agent )
{
    emit AgentOutOfGas( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentRefueled
/** @param  pAgent 
*/
// Created: APE 2004-09-30
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentRefueled( MOS_Agent& agent )
{
    emit AgentRefueled( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentConflictStarted
/** @param  agent 
*/
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentConflictStarted( MOS_Agent& agent )
{
    emit AgentConflictStarted( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentConflictEnded
/** @param  pAgent 
*/
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentConflictEnded( MOS_Agent& agent )
{
    emit AgentConflictEnded( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifySpeedChanged
/** @param  nSpeed 
*/
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_App::NotifySpeedChanged( int nSpeed )
{
    emit SpeedChanged( nSpeed );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentKnowledgeCreated
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge )
{
    emit AgentKnowledgeCreated( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentKnowledgeUpdated
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentKnowledgeUpdated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge )
{
    emit AgentKnowledgeUpdated( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyAgentKnowledgeDeleted
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyAgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge )
{
    emit AgentKnowledgeDeleted( gtia, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyObjectKnowledgeCreated
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyObjectKnowledgeCreated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeCreated( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyObjectKnowledgeUpdated
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyObjectKnowledgeUpdated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeUpdated( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyObjectKnowledgeDeleted
/** @param  gtia 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_App::NotifyObjectKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge )
{
    emit ObjectKnowledgeDeleted( team, knowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::NotifyTacticalLineDeleted
/** @param  line 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_App::NotifyTacticalLineDeleted( MOS_TacticalLine_ABC& line )
{
    emit TacticalLineDeleted( line );
}


// -----------------------------------------------------------------------------
// Name: MOS_App::SetSplashText
/** @param  strText 
*/
// Created: APE 2004-03-23
// -----------------------------------------------------------------------------
void MOS_App::SetSplashText( const QString& strText )
{
    assert( pSplashScreen_ != 0 );
    pSplashScreen_->message( strText, Qt::AlignBottom | Qt::AlignHCenter, Qt::white );
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetResourceName
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string MOS_App::GetResourceName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = resourcesNameMap_.find( id );
    if( found != resourcesNameMap_.end() )
        return found->second;
    else
        return itostring( id );
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetEquipmentName 
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string MOS_App::GetEquipmentName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = equipementNameMap_.find( id );
    if( found != equipementNameMap_.end() )
        return found->second;
    else
        return itostring( id );    
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetEquipementID
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
uint MOS_App::GetEquipementID( const std::string& strName ) const
{
    for( CIT_MosId_String_Map it = equipementNameMap_.begin(); it != equipementNameMap_.end(); ++it )
        if( it->second == strName )
            return it->first;
    return (unsigned)(-1);
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetBreakDownName
// Created: JVT 04-04-09
//-----------------------------------------------------------------------------
std::string MOS_App::GetBreakDownName( uint id ) const
{
    CIT_MosId_String_Map found = breakDownsNameMap_.find( id );
    if( found != breakDownsNameMap_.end() )
        return found->second;
    else
        return itostring( id );    
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetRessourceID
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
unsigned int MOS_App::GetRessourceID( const std::string& strRessource ) const
{
    for( CIT_MosId_String_Map it = resourcesNameMap_.begin(); it != resourcesNameMap_.end(); ++it )
        if( it->second == strRessource )
            return it->first;
    return (unsigned)(-1);
}

// -----------------------------------------------------------------------------
// Name: MOS_App::Is3D
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
bool MOS_App::Is3D()
{
    return pMainWindow_ && pMainWindow_->GetOptions().b3D_;
}
