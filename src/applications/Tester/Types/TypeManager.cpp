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

#include "Tester_pch.h"
#include "TypeManager.h"
#include "Entities/PawnType.h"
#include "Entities/PopulationType.h"
#include "Entities/AutomatType.h"
#include "Entities/PawnModel.h"
#include "Entities/AutomatModel.h"
#include "Entities/PopulationModel.h"

// -----------------------------------------------------------------------------
// Name: TypeManager constructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
TypeManager::TypeManager( const std::string& strModelFile, const std::string& strPawnTypeFile, const std::string& strAutomatTypeFile, const std::string& strPopulationTypeFile )
    : automatTypes_     ()
    , automatModels_    ()
    , pawnTypes_        ()
    , pawnModels_       ()
    , populationTypes_  ()
    , populationModels_ ()
{
    LoadModels         ( strModelFile          );
    LoadPawnTypes      ( strPawnTypeFile       );
    LoadAutomatTypes   ( strAutomatTypeFile    );
    LoadPopulationTypes( strPopulationTypeFile );
}

// -----------------------------------------------------------------------------
// Name: TypeManager destructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
TypeManager::~TypeManager()
{
    for( CIT_AutomatTypeMap it = automatTypes_.begin(); it != automatTypes_.end(); ++it )
        delete it->second;
    automatTypes_.clear();

    for( CIT_PawnTypeMap it = pawnTypes_.begin(); it != pawnTypes_.end(); ++it )
        delete it->second;
    pawnTypes_.clear();

    for( CIT_PopulationTypeMap it = populationTypes_.begin(); it != populationTypes_.end(); ++it )
        delete it->second;
    populationTypes_.clear();

    for( CIT_AutomatModelMap it = automatModels_.begin(); it != automatModels_.end(); ++it )
        delete it->second;
    automatModels_.clear();

    for( CIT_PawnModelMap it = pawnModels_.begin(); it != pawnModels_.end(); ++it )
        delete it->second;
    pawnModels_.clear();

    for( CIT_PopulationModelMap it = populationModels_.begin(); it != populationModels_.end(); ++it )
        delete it->second;
    populationModels_.clear();
}

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadAutomatTypes
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadAutomatTypes( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nAutomatTypesLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Automates" );
        while( archive.NextListElement() )
        {
            archive.Section( "Unite" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            AutomatType* pAutomatType = new AutomatType( *this, strName, archive );
            automatTypes_[ pAutomatType->GetName() ] = pAutomatType;

            archive.EndSection(); // Unite

            nAutomatTypesLoaded++;
        }
        archive.EndList    (); // Automates

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nAutomatTypesLoaded << " Automat types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadPawnTypes
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadPawnTypes( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nPawnTypesLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Pions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Unite" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PawnType* pPawnType = new PawnType( *this, strName, archive );
            pawnTypes_[ pPawnType->GetName() ] = pPawnType;

            archive.EndSection(); // Unite

            nPawnTypesLoaded++;
        }
        archive.EndList    (); // Pions

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nPawnTypesLoaded << " Pawn types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadPopulationTypes
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadPopulationTypes( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nPopulationTypesLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Populations" );
        while( archive.NextListElement() )
        {
            archive.Section( "Population" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PopulationType* pPopulationType = new PopulationType( *this, strName, archive );
            populationTypes_[ pPopulationType->GetName() ] = pPopulationType;

            archive.EndSection(); // Population

            nPopulationTypesLoaded++;
        }
        archive.EndList    (); // Populations

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nPopulationTypesLoaded << " Population types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadModels
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void TypeManager::LoadModels( const std::string& strConfigFile )
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

        LoadPawnModels      ( strFileName );
        LoadAutomatModels   ( strFileName );
        LoadPopulationModels( strFileName );

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

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadAutomatModels
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadAutomatModels( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nAutomatModelsLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Automates" );
        while( archive.NextListElement() )
        {
            archive.Section( "Modele" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            AutomatModel* pAutomatModel = new AutomatModel( strName, archive );
            automatModels_.insert( std::make_pair( strName, pAutomatModel ) );

            archive.EndSection(); // Modele

            nAutomatModelsLoaded++;
        }
        archive.EndList    (); // Automates

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nAutomatModelsLoaded << " Automat models loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TypeManager::LoadPawnModels
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadPawnModels( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
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
            pawnModels_.insert( std::make_pair( strName, pPawnModel ) );

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
// Name: TypeManager::LoadPopulationModels
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void TypeManager::LoadPopulationModels( const std::string& strConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strConfigFile, strDir  );
        MT_ExtractFileName  ( strConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nPopulationModelsLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Populations" );
        while( archive.NextListElement() )
        {
            archive.Section( "Modele" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PopulationModel* pPopulationModel = new PopulationModel( strName, archive );
            populationModels_.insert( std::make_pair( strName, pPopulationModel ) );

            archive.EndSection(); // Modele

            nPopulationModelsLoaded++;
        }
        archive.EndList    (); // Pions

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nPopulationModelsLoaded << " Population models loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const AutomatType* TypeManager::FindAutomatType( const std::string& strName ) const
{
    CIT_AutomatTypeMap it = automatTypes_.find( strName );
    return it == automatTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const AutomatType* TypeManager::FindAutomatType( ASN1T_TypeAutomate asnId ) const
{
    for( CIT_AutomatTypeMap it = automatTypes_.begin(); it != automatTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PawnType* TypeManager::FindPawnType( const std::string& strName ) const
{
    CIT_PawnTypeMap it = pawnTypes_.find( strName );
    return it == pawnTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PawnType* TypeManager::FindPawnType( ASN1T_TypePion asnId ) const
{
    for( CIT_PawnTypeMap it = pawnTypes_.begin(); it != pawnTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PopulationType* TypeManager::FindPopulationType( const std::string& strName ) const
{
    CIT_PopulationTypeMap it = populationTypes_.find( strName );
    return it == populationTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PopulationType* TypeManager::FindPopulationType( ASN1T_TypePion asnId ) const
{
    for( CIT_PopulationTypeMap it = populationTypes_.begin(); it != populationTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const AutomatModel* TypeManager::FindAutomatModel( const std::string& strModel ) const
{
    CIT_AutomatModelMap it = automatModels_.find( strModel );
    if( it != automatModels_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PawnModel* TypeManager::FindPawnModel( const std::string& strModel ) const
{
    CIT_PawnModelMap it = pawnModels_.find( strModel );
    if( it != pawnModels_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TypeManager::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
const PopulationModel* TypeManager::FindPopulationModel( const std::string& strModel ) const
{
    CIT_PopulationModelMap it = populationModels_.find( strModel );
    if( it != populationModels_.end() )
        return it->second;
    return 0;
}