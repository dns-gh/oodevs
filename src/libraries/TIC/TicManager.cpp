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
// $Archive: /MVW_v10/Build/SDK/TIC/src/TicManager.cpp $
// $Author: Sbo $
// $Modtime: 5/07/05 11:54 $
// $Revision: 18 $
// $Workfile: TicManager.cpp $
//
// *****************************************************************************
#include <time.h>

#include "TIC_pch.h"
#include "TicManager.h"

#include "Network/NetworkManager.h"
#include "Entities/EntityManager.h"
#include "Tools/PositionManager.h"
#include "Communications/CommunicationManager.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TIC;

TicManager* TicManager::pTicManager_;

//-----------------------------------------------------------------------------
// Name: TicManager::Initialize
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void TicManager::Initialize( TICExport::Callback_ABC& callback, 
                             const std::string&       strServer, 
                             uint                     nServerPort, 
                             const std::string&       strScipioConfigFile,
                             const std::string&       strPlatformNatureFile )
{
    pTicManager_ = new TicManager( callback, strServer, nServerPort, strScipioConfigFile, strPlatformNatureFile );
    assert( pTicManager_ );
    pTicManager_->pNetworkManager_->Connect();

    CommunicationManager::Initialize();
}

//-----------------------------------------------------------------------------
// Name: TicManager::Terminate
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void TicManager::Terminate()
{
    CommunicationManager::Terminate();

    if( GetTicManager().GetNetworkManager().IsConnected() )
        GetTicManager().GetNetworkManager().Disconnect();
    assert( pTicManager_ );
    delete pTicManager_;

    PositionManager::Terminate();
}

//-----------------------------------------------------------------------------
// Name: TicManager::TicManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
TicManager::TicManager( TICExport::Callback_ABC& callback, 
                        const std::string&       strServer, 
                        uint                     nServerPort, 
                        const std::string&       strScipioConfigFile,
                        const std::string&       strPlatformNatureFile )
    : callback_         ( callback )
    , pNetworkManager_  ( 0 )
    , pEntityManager_   ( 0 )
    , nTick_            ( 0 )
    , nTickDuration_    ( 0 )
    , nCurrentSimTime_  ( 0 )
{
    // entity manager
    pEntityManager_ = new EntityManager();
    assert( pEntityManager_ );
    
    LoadScipioConfigFile( strScipioConfigFile );
    // load TIC-specific platform natures
    pEntityManager_->InitializePlatformNatures( strPlatformNatureFile );

    // network manager
    pNetworkManager_ = new NetworkManager( strServer, nServerPort );
    assert( pNetworkManager_ );
}

//-----------------------------------------------------------------------------
// Name: TicManager::~TicManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
TicManager::~TicManager()
{
	if ( pNetworkManager_ )
		delete pNetworkManager_;
	if ( pEntityManager_ )
		delete pEntityManager_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::Update
// Created: SBO 2005-05-13
// @return true if something a new export must be done (something updated or new tick)
//-----------------------------------------------------------------------------
void TicManager::Update()
{
    try
    {
        pNetworkManager_->Update();    
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "Tic - Network error - " << exception.what() );
        throw;
    }
}



//-----------------------------------------------------------------------------
// Name: TicManager::LoadScipioConfigFile
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void TicManager::LoadScipioConfigFile( const std::string& strScipioConfigFile )
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
        MT_LOG_ERROR_MSG( exception.what() << "Tic - Parse error" );
        throw;
    }
}

