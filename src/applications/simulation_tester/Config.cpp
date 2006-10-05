// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-24 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Config.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
Config::Config( const std::string& strFile )
    : strServer_           ( "127.0.0.1" )
    , strConfigFile_       ( "./scipio.xml" )
    , nPort_               ( 10001 )
    , nTimeFactor_         ( 40 )
    , nPeriod_             ( 180 )
    , nRecompletionPeriod_ ( 180 )
    , nItNumber_           ( 1 )
    , nItInterval_         ( 1 )
    , nMaxMissionPerTick_  ( std::numeric_limits< uint >::max() )
    , strLogFile_          ( "./test.log" )
    , strRandomSeedFile_   ( "./test_seed.xml" )
    , bRecover_            ( false )
    , nRecoveryTick_       ( 0 )
    , strRecoveryFile_     ( "./test_recover.xml" )
    , eTestedEntities_     ( ePawns )
{
    LoadConfigFile( strFile );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::LoadConfigFile
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void Config::LoadConfigFile( const std::string& strConfigFile )
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
        archive.Section   ( "Tester" );

        std::string       strTmp;

        archive.Section( "Sim" );
        archive.ReadField( "Server"    , strServer_     );
        archive.ReadField( "Port"      , nPort_         );
        archive.ReadField( "ConfigFile", strConfigFile_ );
        archive.ReadField( "TimeFactor", nTimeFactor_   );
        archive.EndSection(); // Sim

        archive.Section( "Actions" );
        archive.ReadField( "Entities", strTmp );
        if( !stricmp( strTmp.c_str(), "automats" ) )
            eTestedEntities_ = eAutomats;
        else if( !stricmp( strTmp.c_str(), "populations" ) )
            eTestedEntities_ = ePopulations;

        archive.ReadField( "Period"                , nPeriod_             );
        archive.ReadField( "NumberOfSameMissions"  , nItNumber_           );
        archive.ReadField( "IntervalOfSameMissions", nItInterval_         );
        archive.ReadField( "MaxNumberOfMission"    , nMaxMissionPerTick_  );
        archive.ReadField( "RecompletionPeriod"    , nRecompletionPeriod_ );
        archive.ReadField( "LogFile"               , strLogFile_          );
        archive.ReadField( "RandomSeedFile"        , strRandomSeedFile_   );
        archive.Section( "Recovery" );
        archive.ReadField( "Recover", bRecover_       );
        archive.ReadField( "File"   , strRecoveryFile_ );
        if( bRecover_ )
            LoadRecoveryFile( strRecoveryFile_ );
        archive.EndSection(); // Recover
        archive.EndSection(); // Actions

        archive.EndSection(); // Tester
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
// Name: Config::LoadRecoveryFile
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Config::LoadRecoveryFile( const std::string& strConfigFile )
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
        archive.Section   ( "Recovery" );
        archive.ReadField( "LastTick", nRecoveryTick_ );
        archive.EndSection(); // Recover
        archive.Close     ();
        MT_ChangeDir      ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir      ( strCurrentDir );
        MT_LOG_WARNING_MSG( exception.what() );
        MT_LOG_INFO_MSG   ( "Cannot load recover file, starting tests from beginning" );
    }
}