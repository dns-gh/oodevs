// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_Pch.h"
#include "PawnModel.h"
#include "Actions/Missions/Mission_Pawn_Type.h"

using namespace TEST;

PawnModel::T_PawnModelMap PawnModel::models_;

// -----------------------------------------------------------------------------
// Name: PawnModel constructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PawnModel::PawnModel( const std::string& strName, XmlInputArchive& archive )
    : strName_      ( strName )
    , missionTypes_ ()
{
    try   
    {
        std::string strValue;

        archive.BeginList( "Missions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Mission" );
            std::string strMissionName;
            archive.ReadAttribute( "nom", strMissionName );
            const Mission_Pawn_Type* missionType = Mission_Pawn_Type::Find( strMissionName );
            if( missionType )
                missionTypes_.push_back( missionType );
            else
                MT_LOG_WARNING_MSG( "Reading models: mission type \"" << strMissionName << "\" not found" );
            archive.EndSection(); // Mission
        }
        archive.EndList(); // Missions
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnModel destructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PawnModel::~PawnModel()
{
    missionTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PawnModel::Initialize
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void PawnModel::Initialize( const std::string& strScipioModelFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strScipioModelFile, strDir  );
        MT_ExtractFileName  ( strScipioModelFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nPawnModelsLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Pions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Modele" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PawnModel* pPawnModel = new PawnModel( strName, archive );
            models_.insert( std::make_pair( strName, pPawnModel ) );

            archive.EndSection(); // Modele

            nPawnModelsLoaded++;
        }
        archive.EndList    (); // Pions

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nPawnModelsLoaded << " Pawn models loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnModel::Terminate
// Created: SBO 2005-05-24
// -----------------------------------------------------------------------------
void PawnModel::Terminate()
{
    for( CIT_PawnModelMap it = models_.begin(); it != models_.end(); ++it )
        delete it->second;
    models_.clear();
}

// -----------------------------------------------------------------------------
// Name: PawnModel::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PawnModel* PawnModel::Find( const std::string& strModel )
{
    CIT_PawnModelMap it = models_.find( strModel );
    if( it != models_.end() )
        return it->second;
    return 0;
}
