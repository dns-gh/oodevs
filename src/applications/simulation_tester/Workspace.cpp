// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Workspace.cpp $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 18 $
// $Workfile: Workspace.cpp $
//
// *****************************************************************************
#include <time.h>

#include "simulation_tester_pch.h"
#include "Workspace.h"
#include "Config.h"

#include "Network/NetworkManager.h"
#include "Entities/EntityManager.h"
#include "Tools/PositionManager.h"
#include "Types/TypeManager.h"
#include "Types/TacticalLineManager.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Actions/Missions/Mission_Automat_Type.h"
#include "Actions/Missions/Mission_Population_Type.h"
#include "TestSets/TestSet_ABC.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "MT/MT_XmlTools/MT_XXmlOutputArchive.h"

using namespace TEST;

Workspace* Workspace::pWorkspace_;

//-----------------------------------------------------------------------------
// Name: Workspace::Workspace
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Workspace::Workspace( TestSet_ABC& testSet, const Config& config )
    : pNetworkManager_  ( 0 )
    , pEntityManager_   ( 0 )
    , pTypeManager_     ( 0 )
    , pPositionManager_ ( 0 )
    , pTacticalLineManager_ ( 0 )
    , pScheduler_       ( 0 )
    , pTestSet_         ( &testSet )
    , nTick_            ( 0 )
    , nTickDuration_    ( 0 )
    , nCurrentSimTime_  ( 0 )
    , nTimeFactor_      ( config.GetTimeFactor() )
    , strRecoveryFile_  ( config.GetRecoveryFile() )
{
    InitializeRandomSeed( config.GetRandomSeedFile() );

    // scheduler
    pScheduler_ = new Scheduler( config );

    Mission_Pawn_Type::Initialize();
    Mission_Automat_Type::Initialize();
    Mission_Population_Type::Initialize();
    LoadScipioConfigFile( config.GetConfigFile() );

    std::string strTacticalLineFile;
    MT_ExtractFilePath( config.GetConfigFile(), strTacticalLineFile  );
    pTacticalLineManager_->LoadTacticalLines( strTacticalLineFile + "TacticalLines.xml" );

    // network manager
    pNetworkManager_ = new NetworkManager( *this, config.GetServer(), config.GetPort() );
    assert( pNetworkManager_ );
    pNetworkManager_->Connect();

    pWorkspace_ = this;
}

//-----------------------------------------------------------------------------
// Name: Workspace::~Workspace
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Workspace::~Workspace()
{
    Mission_Automat_Type::Terminate();
    Mission_Pawn_Type::Terminate();
    if( pNetworkManager_->IsConnected() )
        pNetworkManager_->Disconnect();
	delete pNetworkManager_;
    delete pScheduler_;
    delete pTypeManager_;
    delete pPositionManager_;
    delete pTacticalLineManager_;
    delete pEntityManager_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::Update
// Created: SBO 2005-05-13
//-----------------------------------------------------------------------------
void Workspace::Update()
{
    pNetworkManager_->Update();    
}

//-----------------------------------------------------------------------------
// Name: Workspace::LoadScipioConfigFile
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void Workspace::LoadScipioConfigFile( const std::string& strScipioConfigFile )
{
    std::string           strCurrentDir = MT_GetCurrentDir();
    std::string           strDir;
    std::string           strFile;

    MT_ExtractFilePath    ( strScipioConfigFile, strDir  );
    MT_ExtractFileName    ( strScipioConfigFile, strFile );

    try
    {
        XmlInputArchive   archive;

        MT_ChangeDir      ( strDir );
        archive.Open      ( strFile );
        archive.Section   ( "Donnees" );

        std::string       strFileName, strFileName2, strFileName3, strFileName4;

        // entity position manager
        archive.ReadField( "Terrain"  , strFileName );
        pPositionManager_     = new PositionManager( strFileName );
        pTacticalLineManager_ = new TacticalLineManager( *pPositionManager_ );

        archive.ReadField( "Decisionnel", strFileName  );
        archive.ReadField( "Pions"      , strFileName2 );
        archive.ReadField( "Automates"  , strFileName3 );
        archive.ReadField( "Populations", strFileName4 );
        pTypeManager_         = new TypeManager( strFileName, strFileName2, strFileName3, strFileName4 );
        pEntityManager_       = new EntityManager();

        archive.EndSection(); // Donnees
        archive.Close     ();

        MT_ChangeDir      ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir    ( strCurrentDir );
        MT_LOG_ERROR_MSG( exception.what() << "Parse error" );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: Workspace::InitializeRandomSeed
// Created: SBO 2005-08-29
// -----------------------------------------------------------------------------
void Workspace::InitializeRandomSeed( const std::string& strConfigFile )
{
    std::string           strCurrentDir = MT_GetCurrentDir();
    std::string           strDir;
    std::string           strFile;

    MT_ExtractFilePath    ( strConfigFile, strDir  );
    MT_ExtractFileName    ( strConfigFile, strFile );

    try
    {
        XmlInputArchive   archive;
        MT_ChangeDir      ( strDir );
        archive.Open      ( strFile );
        archive.Section   ( "Random" );
        uint nSeed = 0;
        archive.ReadField( "Seed", nSeed );
        srand( nSeed );
        MT_LOG_INFO_MSG( "Using random seed from file: " << nSeed );
        archive.EndSection(); // Random
        archive.Close     ();
        MT_ChangeDir      ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir    ( strCurrentDir );
        MT_LOG_WARNING_MSG( exception.what() );
        uint nSeed = time( 0 );
        srand( nSeed );
        MT_LOG_INFO_MSG( "Using time as random seed: " << nSeed );

        MT_XXmlOutputArchive archive;
        archive.Section   ( "Random" );
        archive.WriteField( "Seed", nSeed );
        archive.EndSection(); // Random

        MT_ChangeDir       ( strDir );
        archive.WriteToFile( strFile );
        MT_ChangeDir       ( strCurrentDir );
    }
}

// -----------------------------------------------------------------------------
// Name: Workspace::SetTimeFactor
// Created: SBO 2005-08-22
// -----------------------------------------------------------------------------
void Workspace::SetTimeFactor( uint32 nFactor /* = 0 */ )
{
    if( nFactor == 0 )
        nFactor = nTimeFactor_;

    MOS_ASN_MsgCtrlChangeTimeFactor asnMsg;
    asnMsg.GetAsnMsg() = nFactor;
    asnMsg.Send();

    std::stringstream strMsg;
    strMsg << "Changing speed to " << nFactor;
    MT_LOG_INFO_MSG( strMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: Workspace::SaveRecoveryPoint
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Workspace::SaveRecoveryPoint()
{
    std::string           strCurrentDir = MT_GetCurrentDir();
    std::string           strDir;
    std::string           strFile;

    MT_ExtractFilePath    ( strRecoveryFile_, strDir  );
    MT_ExtractFileName    ( strRecoveryFile_, strFile );

    try
    {
        MT_XXmlOutputArchive archive;
        archive.Section   ( "Recovery" );
        archive.WriteField( "LastTick", GetScheduler().GetCurrentTick() );
        archive.EndSection(); // Recovery

        MT_ChangeDir       ( strDir );
        archive.WriteToFile( strFile );
        MT_ChangeDir       ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_INFO_MSG( exception.what() );
    }
}
