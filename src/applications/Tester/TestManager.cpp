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
// $Archive: /MVW_v10/Build/SDK/TIC/src/TestManager.cpp $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 18 $
// $Workfile: TestManager.cpp $
//
// *****************************************************************************
#include <time.h>

#include "Tester_Pch.h"
#include "TestManager.h"

#include "Network/NetworkManager.h"
#include "Entities/EntityManager.h"
#include "Tools/PositionManager.h"
#include "Actions/Scheduler.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

TestManager* TestManager::pTestManager_;

//-----------------------------------------------------------------------------
// Name: TestManager::Initialize
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void TestManager::Initialize( TestSet_ABC*             pTestSet,
                              const std::string&       strServer, 
                              uint                     nServerPort, 
                              const std::string&       strScipioConfigFile )
{
    pTestManager_ = new TestManager( pTestSet, strServer, nServerPort, strScipioConfigFile );
    assert( pTestManager_ );
    pTestManager_->pNetworkManager_->Connect();
}

//-----------------------------------------------------------------------------
// Name: TestManager::Terminate
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void TestManager::Terminate()
{
    if( GetTestManager().GetNetworkManager().IsConnected() )
        GetTestManager().GetNetworkManager().Disconnect();
    assert( pTestManager_ );
    delete pTestManager_;

    PositionManager::Terminate();
}

//-----------------------------------------------------------------------------
// Name: TestManager::TestManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
TestManager::TestManager( TestSet_ABC*             pTestSet,
                          const std::string&       strServer, 
                          uint                     nServerPort, 
                          const std::string&       strScipioConfigFile )
    : pNetworkManager_  ( 0 )
    , pEntityManager_   ( 0 )
    , pScheduler_       ( 0 )
    , pTestSet_         ( pTestSet )
    , nTick_            ( 0 )
    , nTickDuration_    ( 0 )
    , nCurrentSimTime_  ( 0 )
{
    // scheduler
    pScheduler_ = new Scheduler();

    // entity manager
    pEntityManager_ = new EntityManager();
    assert( pEntityManager_ );
    
    LoadScipioConfigFile( strScipioConfigFile );

    // network manager
    pNetworkManager_ = new NetworkManager( strServer, nServerPort );
    assert( pNetworkManager_ );
}

//-----------------------------------------------------------------------------
// Name: TestManager::~TestManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
TestManager::~TestManager()
{
    if( pScheduler_ )
        delete pScheduler_;
	if ( pNetworkManager_ )
		delete pNetworkManager_;
	if ( pEntityManager_ )
		delete pEntityManager_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::Update
// Created: SBO 2005-05-13
// @return true if something a new export must be done (something updated or new tick)
//-----------------------------------------------------------------------------
void TestManager::Update()
{
    try
    {
        pNetworkManager_->Update();    
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "Network error - " << exception.what() );
        throw;
    }
}



//-----------------------------------------------------------------------------
// Name: TestManager::LoadScipioConfigFile
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void TestManager::LoadScipioConfigFile( const std::string& strScipioConfigFile )
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

        std::string       strConfigFile;
        std::string       strConfigFile2;
        std::string       strConfigFile3;

        // entity position manager
        archive.ReadField          ( "Terrain"    , strConfigFile );
        PositionManager::Initialize( strConfigFile );

        pEntityManager_->Initialize( archive );

        archive.EndSection(); // Donnees
        archive.Close     ();

        MT_ChangeDir      ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir    ( strCurrentDir );
        MT_LOG_ERROR_MSG( exception.what().c_str() << "Parse error" );
        throw;
    }
}

