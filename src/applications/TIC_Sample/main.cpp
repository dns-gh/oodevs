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

// Tic Lib
#include "TIC_Export/Workspace.h"

// Local includes
#include "OutputXml.h"

#include <signal.h>

#include <windows.h>

using namespace TICExport;

//-----------------------------------------------------------------------------
// clean exit management
static bool bRun = true;

//-----------------------------------------------------------------------------
// Name: ctrl_c_handler
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void ctrl_c_handler( int /*nCtx*/ )
{
    std::cout << "Exit signal received..." << std::endl;
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

    OutputXml output( "tic.xml" );
    Workspace workspace( output, "127.0.0.1", 10000, "D:/Dev/Build/Data/scipio.xml", "./data/plateformesTic.xml" );
    output.SetWorkspace( workspace );  

    while( bRun )
    {
        workspace.Update();
        Sleep( 0 );
    }
    std::cout << "Program terminated normally" << std::endl;
}


//-----------------------------------------------------------------------------
// Name: main
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
int main( int /*nArgc*/, char** /*ppArgv*/ )
{
    try
    {
        ExportXML();
	}
    catch( ... )
    {
        std::cerr << "Program terminated abnormally " << std::endl;
    }
	return 0;
}