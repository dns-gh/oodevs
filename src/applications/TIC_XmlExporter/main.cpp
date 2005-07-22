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
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/main.cpp $
// $Author: Sbo $
// $Modtime: 22/07/05 10:10 $
// $Revision: 16 $
// $Workfile: main.cpp $
//
// *****************************************************************************
#include "Types.h"

// Tic Lib
#include "Workspace.h"

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"
#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Assert.h"

// Local includes
#include "Config.h"
#include "OutputXml.h"

#include <windows.h>
#include <signal.h>
#include <cstdio>

using namespace TICExport;

//-----------------------------------------------------------------------------
// command line parameters
static std::string strMainConfigFile = "tic_config.xml";

//-----------------------------------------------------------------------------
// clean exit management
static bool bRun = true;

//-----------------------------------------------------------------------------
// Name: ctrl_c_handler
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void ctrl_c_handler( int /*nCtx*/ )
{
    MT_LOG_INFO_MSG( "Exit request received..." );
	bRun = false;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: ExportXML
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void ExportXML()
{
    signal( SIGINT, ctrl_c_handler );

    // Init config
    Config config( strMainConfigFile );

    OutputXml output( config );

    Workspace workspace(  output
                        , config.GetServer                ()
                        , config.GetPort                  ()
                        , config.GetSimConfigFile         ()
                        , config.GetPlatformNatureFilename() );
    output.SetWorkspace( workspace );

    config.LoadDisaggregationParameters( workspace );

    workspace.SetDistanceInterPlatform( Workspace::eFormationDiamond, 200.0 );
    while( bRun )
    {
        workspace.Update();
        //Sleep( 0 );
    }
    MT_LOG_INFO_MSG( "Program terminated normally" );
}

// -----------------------------------------------------------------------------
// Name: ProcessCommandLineArguments
// Created: SBO 2005-07-22
// -----------------------------------------------------------------------------
void ProcessCommandLineArguments( int nArgc, char** ppArgv )
{
    for( int i = 1; i < nArgc; ++i )
    {
        if( !strcmp( ppArgv[ i ], "-c" ) )
        {
            if( ++i == nArgc )
                throw std::exception( "-c : missing config file path." );
            strMainConfigFile = std::string( ppArgv[ i ] );
        }
        else
        {
            std::stringstream ss;
            ss << ppArgv[ i ] << " : invalid argument.";
            throw std::exception( ss.str().c_str() );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: main
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
int main( int nArgc, char** ppArgv )
{
    // enable logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    try
    {
        ProcessCommandLineArguments( nArgc, ppArgv );
        ExportXML();
	}
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
        MT_LOG_ERROR_MSG( "Program terminated abnormally" );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Program terminated abnormally" );
    }

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
	return 0;
}