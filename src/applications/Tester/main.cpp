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
#include "Types.h"

#include "Workspace.h"

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

#include "TestSets/TestSet_Simple.h"
#include "TestSets/TestSet_PawnMissions.h"
#include "TestSets/TestSet_AllPawnMissions.h"
#include "TestSets/TestSet_AllAutomatMissions.h"

using namespace TEST;

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
// Name: Run
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Run()
{
    signal( SIGINT, ctrl_c_handler );

    //TestSet_AllAutomatMissions tests;
    TestSet_AllPawnMissions tests;

    Workspace ws( &tests, "127.0.0.1", 10000, "D:/Dev/scipio-svn/data/test/scipio.xml" );

    while( bRun )
    {
        ws.Update();
        Sleep( 0 );
    }

    MT_LOG_INFO_MSG( "Program terminated normally" );
}


//-----------------------------------------------------------------------------
// Name: main
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
int main( int /*nArgc*/, char** /*ppArgv*/ )
{
    // enable logger
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    try
    {
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