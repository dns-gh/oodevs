// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/EntityManager.cpp $
// $Author: Sbo $
// $Modtime: 7/07/05 15:09 $
// $Revision: 17 $
// $Workfile: EntityManager.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"

#include "Entities/EntityManager.h"

#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Object.h"
#include "Entities/TacticalLines/TacticalLineManager.h"

#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"
#include "Entities/Types/AutomatModel.h"
#include "Entities/Types/PawnModel.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: EntityManager::EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::EntityManager()
{
    // entities 
    Automat            ::Initialize();
    Pawn               ::Initialize();
    Team               ::Initialize();
    KnowledgeGroup     ::Initialize();
    Object             ::Initialize();
}

//-----------------------------------------------------------------------------
// Name: EntityManager::~EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    // entity types
    AutomatType        ::Terminate();
    PawnType           ::Terminate();

    // entities
    Automat            ::Terminate();
    Pawn               ::Terminate();
    Team               ::Terminate();
    KnowledgeGroup     ::Terminate();
    Object             ::Terminate();
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Initialize
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void EntityManager::Initialize( XmlInputArchive& archive ) const
{
    try
    {
        std::string strFile;

        archive.ReadField( "Decisionnel", strFile );
        InitializeModels ( strFile );

        archive.ReadField( "Pions"      , strFile );
        PawnType    ::Initialize( strFile );

        archive.ReadField( "Automates"  , strFile );
        AutomatType ::Initialize( strFile  );
    }
    catch( ... )
    {
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityManager::EntityManager::InitializeModels
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void EntityManager::InitializeModels( const std::string& strConfigFile ) const
{
    try
    {
        std::string       strCurrentDir = MT_GetCurrentDir();
        std::string       strDir;
        std::string       strFile;
        MT_ExtractFilePath( strConfigFile, strDir  );
        MT_ExtractFileName( strConfigFile, strFile );
        MT_ChangeDir      ( strDir );

        XmlInputArchive   archive;
        
        archive.Open      ( strFile );

        archive.Section   ( "DirectIA" );
        std::string strFileName;
        archive.ReadField ( "Modeles", strFileName );

        PawnModel   ::Initialize( strFileName );
        //AutomatModel::Initialize( strFileName );

        archive.EndSection(); // DirectIA

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}