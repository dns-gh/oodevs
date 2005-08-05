//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_App.cpp $
// $Author: Age $
// $Modtime: 11/03/05 16:59 $
// $Revision: 43 $
// $Workfile: MOS_App.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_App.h"
#include "moc_MOS_App.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_App.inl"
#endif

#include "MOS_MOSServer.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentManager.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_WorldWidget.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_TestManager.h"
#include "MOS_History.h"
#include "MOS_SensorType.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"
#include "MOS_Experience.h"

#include <direct.h>
#include <cstdio>
#include <qtimer.h>

#include "MT_Tools/MT_ScipioException.h"


MOS_App* MOS_App::pTheApp_ = 0;

//-----------------------------------------------------------------------------
// Name: MOS_App constructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MOS_App::MOS_App( int nArgc, char** ppArgv )
    : QApplication  ( nArgc, ppArgv )
    , bRunning_     ( false )
    , nTimeSeconds_ ( 0 )
    , pTestManager_ ( 0 )
    , pMOSServer_( 0 )
    , pWorld_( 0 )
    , pAgentManager_( 0 )
    , pLineManager_( 0 )
    , pMainWindow_( 0 )
    , pDynaObjectManager_( 0 )
    , pWeatherManager_( 0 )
{
    // manage command line options
    MT_CommandLine cmdLine( argc(), argv() );
    const std::string strConfFile = cmdLine.GetOptionStrValue( "-conffile", "./scipio.xml" );
    MT_ExtractFilePath( strConfFile, strRootDirectory_  );
    MT_ExtractFileName( strConfFile, strRootConfigFile_ ); 
}

//-----------------------------------------------------------------------------
// Name: MOS_App destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MOS_App::~MOS_App()
{
    Terminate();
    MT_TextOutputArchive outputArchive;
    for( IT_StringVector it = pointsToSave_.begin(); it != pointsToSave_.end(); ++it )
        outputArchive << (*it) << "\n";

    outputArchive.WriteToFile( "points.txt" );

    delete pDynaObjectManager_;
    delete pAgentManager_;
    delete pLineManager_;
    delete pWorld_;
    delete pMOSServer_;
    delete pMainWindow_;
}

// -----------------------------------------------------------------------------
// Name: MOS_App::HideEverything
// Created: AGE 2005-03-11
// -----------------------------------------------------------------------------
void MOS_App::HideEverything()
{
    if( pTheApp_ && mainWidget() )
        mainWidget()->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeRawVisionData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeRawVisionData( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTerFile;
    if( !scipioArchive.ReadField( "Terrain", strTerFile ) )
        return;

    std::string strTerrainDataPath;
    MT_ExtractFilePath( strTerFile, strTerrainDataPath );

    MT_XXmlInputArchive terrainArchive;
    if( !terrainArchive.Open( strTerFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !terrainArchive.Section( "Terrain" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strRawVisionDirectory;
    if( !terrainArchive.ReadField( "RawVision", strRawVisionDirectory ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

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
    std::string strTerFile;
    if( !scipioArchive.ReadField( "Terrain", strTerFile ) )
        return;

    // Meteo
    std::string strMeteoFile;
    if( !scipioArchive.ReadField( "Meteo", strMeteoFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    rawData_.InitializeMeteo( strMeteoFile );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeTerrainData
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeTerrainData( MT_InputArchive_ABC& scipioArchive )
{
    std::string strTerFile;
    if( !scipioArchive.ReadField( "Terrain", strTerFile ) )
        return;
    pWorld_ = new MOS_World( strTerFile );
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeRessourceIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeRessourceIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strDotationFile;
    if( !scipioArchive.ReadField( "Dotations", strDotationFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive dotArchive;
    if( !dotArchive.Open( strDotationFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !dotArchive.BeginList( "Dotations" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( dotArchive.NextListElement() )
    {
        if( !dotArchive.Section( "Dotation" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
        if( !dotArchive.BeginList( "Categories" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
        while( dotArchive.NextListElement() )
        {
            if( !dotArchive.Section( "Categorie" ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
            std::string strName;
            uint        nID;
            if( !dotArchive.ReadAttribute( "nom", strName ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
            if( !dotArchive.ReadField( "MosID", nID ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

            ressourcesNameMap_.insert( std::make_pair( nID, strName ) );

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
    if( !scipioArchive.ReadField( "Composantes", strCompFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive compArchive;
    if( !compArchive.Open( strCompFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !compArchive.BeginList( "Composantes" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( compArchive.NextListElement() )
    {
        if( !compArchive.Section( "Composante" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        std::string strName;
        uint        nID;
        if( !compArchive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
        if( !compArchive.ReadField( "MosID", nID ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        equipementNameMap_.insert( std::make_pair( nID, strName ) );

        compArchive.EndSection(); // Composante
    }
    compArchive.EndList(); // Composantes
    compArchive.Close();
}



// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeBreakdownIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeBreakdownIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strCompFile;
    if( !scipioArchive.ReadField( "Pannes", strCompFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive breakdownArchive;
    if( !breakdownArchive.Open( strCompFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !breakdownArchive.Section( "Pannes" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    if( !breakdownArchive.Section( "Types" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    for( uint i = 1; i < 4; ++i )
    {
        std::stringstream strTmp;
        strTmp << "NTI" << i;
        if( !breakdownArchive.BeginList( strTmp.str() ) )
        {
            breakdownArchive.ClearErrors();
            continue;
        }
        while( breakdownArchive.NextListElement() )
        {
            if( !breakdownArchive.Section( "Panne" ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

            std::string strName;
            uint        nID;
            if( !breakdownArchive.ReadAttribute( "nom", strName ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
            if( !breakdownArchive.ReadField( "MosID", nID ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

            breakDownsNameMap_.insert( std::make_pair( nID, strName ) );

            breakdownArchive.EndSection(); // Panne
        }
        breakdownArchive.EndList(); // NTIx
    }

    breakdownArchive.EndSection(); // Types
    breakdownArchive.EndSection(); // Pannes
    breakdownArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeAgentNBCIDs
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_App::InitializeAgentNBCIDs( MT_InputArchive_ABC& scipioArchive )
{
    std::string strNBCFile;
    if( !scipioArchive.ReadField( "NBC", strNBCFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive nbcArchive;
    if( !nbcArchive.Open( strNBCFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !nbcArchive.Section( "NBC" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !nbcArchive.BeginList( "AgentsNBC" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( nbcArchive.NextListElement() )
    {
        if( !nbcArchive.Section( "AgentNBC" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        std::string strName;
        uint        nID;
        if( !nbcArchive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
        if( !nbcArchive.ReadField( "MosID", nID ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        nbcAgentsMap_.insert( std::make_pair( nID, strName ) );

        nbcArchive.EndSection(); // AgentNBC
    }
    nbcArchive.EndList(); // AgentsNBC
    nbcArchive.EndSection(); // NBC
    nbcArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::RegisterSensorType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void MOS_App::RegisterSensorType( const MOS_SensorType& sensor )
{
    const MOS_SensorType*& pSensor = sensorTypes_[ sensor.GetName() ];
    assert( !pSensor );
    pSensor = &sensor; 
}

// -----------------------------------------------------------------------------
// Name: MOS_App::InitializeSensors
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void MOS_App::InitializeSensors( MT_InputArchive_ABC& scipioArchive )
{
    std::string strSensorFile;
    if( !scipioArchive.ReadField( "Capteurs", strSensorFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    MT_XXmlInputArchive sensorArchive;
    if( !sensorArchive.Open( strSensorFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !sensorArchive.BeginList( "Senseurs" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( sensorArchive.NextListElement() )
    {
        if( !sensorArchive.Section( "Senseur" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    
        std::string strName;
        if( !sensorArchive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        if( sensorArchive.Section( "DetectionAgents" ) )
        {
            RegisterSensorType( *new MOS_SensorType( strName, sensorArchive ) );
            sensorArchive.EndSection();
        }
        else
            sensorArchive.ClearErrors();
        sensorArchive.EndSection(); // Senseur
    }

    sensorArchive.EndList(); // Capteurs

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

//-----------------------------------------------------------------------------
// Name: MOS_App::Initialize
// Created: NLD 2002-11-28
// Last modified: JVT 03-12-11
//-----------------------------------------------------------------------------
void MOS_App::Initialize()
{
    pMainWindow_        = new MOS_MainWindow        ();
    pMOSServer_         = new MOS_MOSServer         ();
    pAgentManager_      = new MOS_AgentManager      ();
    pLineManager_       = new MOS_LineManager       ();
    pDynaObjectManager_ = new MOS_DynaObjectManager ();
    pTestManager_       = new MOS_TestManager       ();
    pWeatherManager_    = new MOS_Meteo_Manager();

    const std::string strInitialDir = MT_GetCurrentDir();
    MT_ChangeDir( strRootDirectory_ );

    MT_XXmlInputArchive scipioArchive;
    if ( !scipioArchive.Open( strRootConfigFile_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    if( !scipioArchive.Section( "Scipio" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    if( !scipioArchive.Section( "Donnees" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    InitializeRawVisionData( scipioArchive );   
    InitializeTerrainData  ( scipioArchive );
    InitializeMeteo        ( scipioArchive ); 
    InitializeRessourceIDs ( scipioArchive );
    InitializeEquipementIDs( scipioArchive );
    InitializeAgentNBCIDs  ( scipioArchive );
    InitializeSensors      ( scipioArchive ); 
    InitializeHumanFactors ( scipioArchive ); 
    InitializeBreakdownIDs ( scipioArchive );

    scipioArchive.EndSection(); // Donnees
    scipioArchive.EndSection(); // Scipio

    pMOSServer_        ->Initialize();
    pAgentManager_     ->Initialize();
    pMainWindow_       ->Initialize();
    pLineManager_      ->Initialize();
    pDynaObjectManager_->Initialize();

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetCurrentGL();

    QTimer* pTimer = new QTimer( this );
    connect( pTimer, SIGNAL(timeout()), this, SLOT(OnTimeOut()) );
    connect( this, SIGNAL(lastWindowClosed()), this, SLOT(quit()) );
    pTimer->start( 25 );
    bRunning_ = true;
    setMainWidget( pMainWindow_ );
    pMainWindow_->show();

    MT_ChangeDir( strInitialDir );
}

//-----------------------------------------------------------------------------
// Name: MOS_App::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_App::Terminate()
{
    pTestManager_      ->Terminate();
    pMOSServer_        ->Terminate();
    pMainWindow_       ->Terminate();
    pLineManager_      ->Terminate();
    pDynaObjectManager_->Terminate();
    pAgentManager_     ->Terminate();
}

// -----------------------------------------------------------------------------
// Name: MOS_App::OnTimeOut
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_App::OnTimeOut()
{
    pMOSServer_->Update();
    pTestManager_->Update( nTimeSeconds_ );
}


//=============================================================================
// ACTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_App::NotifyTimeHasChanged
// Created: NLD 2002-09-16
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_App::NotifyTimeHasChanged( uint nTimeInSeconds )
{
    nTimeSeconds_ = nTimeInSeconds;
    pMainWindow_->NotifyTimeHasChanged( nTimeInSeconds );
    rawData_.UpdateEphemeride();
    pMainWindow_->GetHistory().Update();
}


//-----------------------------------------------------------------------------
// Name: MOS_App::SavePoint
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_App::SavePoint( const MT_Vector2D& point )
{
    std::string strPosition;
    GetWorld().SimToMosMgrsCoord( point, strPosition );
    if( std::find( pointsToSave_.begin(), pointsToSave_.end(), strPosition ) == pointsToSave_.end() )
        pointsToSave_.push_back( strPosition );
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string MOS_App::GetRessourceName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = ressourcesNameMap_.find( id );
    if( found != ressourcesNameMap_.end() )
        return found->second;
    else
        return itostring( id );
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
std::string MOS_App::GetEquipementName( MIL_AgentID id ) const
{
    CIT_MosId_String_Map found = equipementNameMap_.find( id );
    if( found != equipementNameMap_.end() )
        return found->second;
    else
        return itostring( id );    
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetEquipementID
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
uint MOS_App::GetEquipementID( const std::string& strName ) const
{
    for( CIT_MosId_String_Map it = equipementNameMap_.begin(); it != equipementNameMap_.end(); ++it )
    {
        if( it->second == strName )
            return it->first;
    }
    return 0;
}



// -----------------------------------------------------------------------------
// Name: MOS_App::GetRessourceID
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
uint MOS_App::GetRessourceID( const std::string& strName ) const
{
    for( CIT_MosId_String_Map it = ressourcesNameMap_.begin(); it != ressourcesNameMap_.end(); ++it )
    {
        if( it->second == strName )
            return it->first;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetAgentNBCName
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
std::string MOS_App::GetAgentNBCName( uint id ) const
{
    CIT_MosId_String_Map found = nbcAgentsMap_.find( id );
    if( found != nbcAgentsMap_.end() )
        return found->second;
    else
        return itostring( id );
}



//-----------------------------------------------------------------------------
// Name: std::string 
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

