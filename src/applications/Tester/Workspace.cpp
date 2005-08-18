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

#include "Tester_Pch.h"
#include "Workspace.h"

#include "Network/NetworkManager.h"
#include "Entities/EntityManager.h"
#include "Tools/PositionManager.h"
#include "Types/TypeManager.h"
#include "Types/TacticalLineManager.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Actions/Missions/Mission_Automat_Type.h"
#include "TestSets/TestSet_ABC.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

Workspace* Workspace::pWorkspace_;

//-----------------------------------------------------------------------------
// Name: Workspace::Workspace
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Workspace::Workspace( TestSet_ABC*       pTestSet,
                      const std::string& strServer, 
                      uint               nServerPort, 
                      const std::string& strScipioConfigFile )
    : pNetworkManager_  ( 0 )
    , pEntityManager_   ( 0 )
    , pTypeManager_     ( 0 )
    , pPositionManager_ ( 0 )
    , pTacticalLineManager_ ( 0 )
    , pScheduler_       ( 0 )
    , pTestSet_         ( pTestSet )
    , nTick_            ( 0 )
    , nTickDuration_    ( 0 )
    , nCurrentSimTime_  ( 0 )
{
    // scheduler
    pScheduler_ = new Scheduler();

    Mission_Pawn_Type::Initialize();
    Mission_Automat_Type::Initialize();
    LoadScipioConfigFile( strScipioConfigFile );

    // network manager
    pNetworkManager_ = new NetworkManager( *this, strServer, nServerPort );
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
    delete pTestSet_;
    delete pScheduler_;
    delete pTypeManager_;
    delete pPositionManager_;
    delete pTacticalLineManager_;
    delete pEntityManager_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::Update
// Created: SBO 2005-05-13
// @return true if something a new export must be done (something updated or new tick)
//-----------------------------------------------------------------------------
void Workspace::Update()
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

        std::string       strFileName, strFileName2, strFileName3;

        // entity position manager
        archive.ReadField( "Terrain"  , strFileName );
        pPositionManager_     = new PositionManager( strFileName );
        pTacticalLineManager_ = new TacticalLineManager( *pPositionManager_ );

        archive.ReadField( "Decisionnel", strFileName );
        archive.ReadField( "Pions"      , strFileName2  );
        archive.ReadField( "Automates"  , strFileName3 );
        pTypeManager_         = new TypeManager( strFileName, strFileName2, strFileName3 );
        pEntityManager_       = new EntityManager();

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

