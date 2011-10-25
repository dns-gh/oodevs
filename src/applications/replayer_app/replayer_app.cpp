// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/win32/FlexLm.h"
#include "tools/WinArguments.h"
#include <windows.h>

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword-replayer", 1.0f, "license.dat;.", silentMode ? FlexLmLicense::eCheckModeSilent : FlexLmLicense::eCheckModeNormal ) );
#endif
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    int nResult = EXIT_SUCCESS;
    try
    {
        App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
        app.Execute();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
        nResult = EXIT_FAILURE;
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
