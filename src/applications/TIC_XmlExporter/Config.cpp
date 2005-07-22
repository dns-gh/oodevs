// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/Config.cpp $
// $Author: Sbo $
// $Modtime: 22/07/05 10:15 $
// $Revision: 11 $
// $Workfile: Config.cpp $
//
// *****************************************************************************

#include <list>
#include <vector>

#include "Config.h"
#include "Workspace.h"

// archive logger
#include "MT_Tools/MT_Assert.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT_Tools/MT_InputArchive_Logger.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

// archive logger XML
class MT_XXmlInputArchive;
typedef MT_InputArchive_Logger< MT_XXmlInputArchive > XmlInputArchive;

using namespace TICExport;

//-----------------------------------------------------------------------------
// Name: Config::Config
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Config::Config( const std::string& strConfigFile )
    : strConfigFile_         ( strConfigFile )
    , strServer_             ()
    , nPort_                 ( 0 )
    , strSimConfigFile_      ()
    , strFilterFilename_     ()
    , strExportFilename_     ()
    , bExportCommunications_ ( true )
    , bExportIncremental_    ( true )
{
    LoadParameters();
}

//-----------------------------------------------------------------------------
// Name: Config::~Config
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Config::~Config()
{
}

//-----------------------------------------------------------------------------
// Name: Config::LoadParameters
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Config::LoadParameters()
{
    XmlInputArchive archive;
    std::string     strDirectory;
    std::string     strFilename;

    MT_ExtractFilePath  ( strConfigFile_, strDirectory  );
    MT_ExtractFileName  ( strConfigFile_, strFilename   );

    const std::string   strCurrentDir = MT_GetCurrentDir();

    try
    {
        MT_ChangeDir( strDirectory );

        archive.Open      ( strFilename );

        archive.Section   ( "ConnexionScipio" );
        archive.ReadField ( "Serveur"             , strServer_                 );
        archive.ReadField ( "Port"                , nPort_                     );
        archive.ReadField ( "FichierConfiguration", strSimConfigFile_          );
        archive.ReadField ( "NaturePlateformes"   , strPlatformNatureFilename_ );
        archive.EndSection(); // ConnexionScipio

        archive.Section   ( "Export" );

        archive.ReadField ( "Fichier"             , strExportFilename_     );
        archive.ReadField ( "Incremental"         , bExportIncremental_    );
        archive.ReadField ( "ExportCommunications", bExportCommunications_ );

        archive.EndSection(); // Export

        archive.Close     ();
        MT_ChangeDir( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what().c_str() );
        MT_ChangeDir( strCurrentDir );
        throw;
    }
}

//-----------------------------------------------------------------------------
// Name: Config::LoadDisaggregationParameters
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Config::LoadDisaggregationParameters( Workspace& workspace )
{
    XmlInputArchive archive;

    std::string     strDirectory;
    std::string     strFilename;

    MT_ExtractFilePath( strConfigFile_, strDirectory  );
    MT_ExtractFileName( strConfigFile_, strFilename   );

    const std::string strCurrentDir = MT_GetCurrentDir();

    try
    {
        if( !strDirectory.empty() && MT_ChangeDir( strDirectory ) != eErrNo_OK )
        {
            std::stringstream ss;
            ss << "Cannot open directory: " << strDirectory;
            throw std::exception( ss.str().c_str() );
        }

        archive.Open      ( strFilename );
        archive.Section   ( "Export" );

        archive.ReadField ( "FichierFiltre", strFilterFilename_, XmlInputArchive::eNothing );

        // set disaggregation filter if specified
        if( !strFilterFilename_.empty() )
            workspace.SetDisaggregationFilterFile( strFilterFilename_ );
        else
            MT_LOG_INFO_MSG( "Disaggregation: no filter file specified in config file" );

        if( archive.BeginList ( "ZonesDesagregees", XmlInputArchive::eNothing ) )
        {
            while( archive.NextListElement() )
            {
                archive.Section( "ZoneDesagregee" );
                if( archive.BeginList( "Points" ) )
                {
                    Workspace::T_DisaggregationArea area;
                    double rLatitude  = 0.0;
                    double rLongitude = 0.0;
                    while( archive.NextListElement() )
                    {
                        archive.Section   ( "Point" );
                        archive.ReadField ( "Latitude" , rLatitude  );
                        archive.ReadField ( "Longitude", rLongitude );
                        archive.EndSection(); // Point
                        area.push_back    ( std::make_pair( rLatitude, rLongitude ) );
                    }
                    archive.EndList(); // Points
                    workspace.AddDisaggregatedArea( area );
                    area.clear();
                }
                archive.EndSection(); // ZoneDesagregee
            }
            archive.EndList   (); // ZonesDesagregees
        }
        else
            MT_LOG_INFO_MSG( "Disaggregation: no area specified in config file" );

        if( archive.BeginList ( "AutomatesDesagreges", XmlInputArchive::eNothing ) )
        {
            uint nId = 0;
            while( archive.NextListElement() )
            {
                archive.Section      ( "AutomateDesagrege" );
                archive.ReadAttribute( "id", nId );
                workspace.AddDisaggregatedAutomat( nId );
                archive.EndSection   (); // AutomateDesagrege
            }
            archive.EndList   (); // AutomatesDesagreges
        }
        else
            MT_LOG_INFO_MSG( "Disaggregation: no automat specified in config file" );

        if( archive.BeginList ( "GroupesDeConnaissanceDesagreges", XmlInputArchive::eNothing ) )
        {
            uint nId = 0;
            while( archive.NextListElement() )
            {
                archive.Section      ( "GroupeDeConnaissanceDesagrege" );
                archive.ReadAttribute( "id", nId );
                workspace.AddDisaggregatedKnowledgeGroup( nId );
                archive.EndSection   (); // GroupeDeConnaissanceDesagrege
            }
            archive.EndList   (); // GroupesDeConnaissanceDesagreges
        }
        else
            MT_LOG_INFO_MSG( "Disaggregation: no knowledge group specified in config file" );

        if( archive.BeginList ( "PionsDesagreges", XmlInputArchive::eNothing ) )
        {
            uint nId = 0;
            while( archive.NextListElement() )
            {
                archive.Section      ( "PionDesagrege" );
                archive.ReadAttribute( "id", nId );
                workspace.AddDisaggregatedPawn( nId );
                archive.EndSection   (); // PionDesagrege
            }
            archive.EndList   (); // PionsDesagreges
        }
        else
            MT_LOG_INFO_MSG( "Disaggregation: no pawn specified in config file" );

        archive.EndSection(); // Export
        archive.Close     ();

        MT_ChangeDir( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir( strCurrentDir );
        MT_LOG_ERROR_MSG( "Parse error " << exception.what().c_str() );
        throw;
    }
    catch( std::exception& exception )
    {
        MT_ChangeDir( strCurrentDir );
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}