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
// $Archive: /MVW_v10/Build/SDK/TIC_Sample/src/main.cpp $
// $Author: Sbo $
// $Modtime: 5/07/05 11:48 $
// $Revision: 15 $
// $Workfile: main.cpp $
//
// *****************************************************************************
#include "Tester_pch.h"
#include "Types.h"

#include "Workspace.h"
#include "Actions/Scheduler.h"
#include "Config.h"

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"
#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Assert.h"

#include <signal.h>
#include <windows.h>

//#include "TestSets/TestSet_Simple.h"
//#include "TestSets/TestSet_PawnMissions.h"
#include "TestSets/TestSet_AllPawnMissions.h"
#include "TestSets/TestSet_AllAutomatMissions.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// clean exit management
bool        bRun          = true;
std::string strConfigFile = "./test.xml";

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

// -----------------------------------------------------------------------------
// Name: ProcessCommandLine
// Created: SBO 2005-08-22
// -----------------------------------------------------------------------------
void ProcessCommandLine( int nArgc, char** ppArgv )
{
    for( int i = 1; i < nArgc; ++i )
    {
        if( !stricmp( ppArgv[ i ], "-conffile" ) && i + 1 <= nArgc )
            strConfigFile = ppArgv[ ++i ];
        else
            MT_LOG_WARNING_MSG( "invalid parameter" << ppArgv[ i ] );
    }
}

//-----------------------------------------------------------------------------
// Name: Run
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Run()
{
    signal( SIGINT, ctrl_c_handler );

    Config config( strConfigFile );

    MT_FileLogger           fileLogger( config.GetLogFile().c_str() );
    MT_LOG_REGISTER_LOGGER( fileLogger );

    TestSet_ABC* pTestSet;
    if( config.MustTestAutomat() )
    {
        pTestSet = new TestSet_AllAutomatMissions( config.GetIterationNumber() );
        MT_LOG_INFO_MSG( "Testing automat missions" );
    }
    else
    {
        pTestSet = new TestSet_AllPawnMissions( config.GetIterationNumber() );
        MT_LOG_INFO_MSG( "Testing pawn missions" );
    }

    MT_LOG_INFO_MSG( "Starting tests on '" << config.GetServer() << ":" << config.GetPort() << 
                     "' with time factor '" << config.GetTimeFactor() << "'" );
    Workspace ws( *pTestSet, config );

    try
    {
        while( bRun )
        {
            ws.Update();
            Sleep( 10 );
        }
        delete pTestSet;
        MT_LOG_INFO_MSG( "Program terminated normally" );
        MT_LOG_UNREGISTER_LOGGER( fileLogger );
    }
    catch( ... )
    {
        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        throw;
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
        ProcessCommandLine( nArgc, ppArgv );
        Run();
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