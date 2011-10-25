// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/win32/FlexLm.h"
#include "tools/WinArguments.h"
#include <windows.h>

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    const std::string licenseFeature = "sword-replayer";
    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    try
    {
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( licenseFeature, 1.0f, "license.dat;.", FlexLmLicense::eCheckModeCustom ) );
#endif
        App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
        app.Execute();
    }
    catch( FlexLmLicense::LicenseError& error )
    {
        license_gui::LicenseDialog::Run( licenseFeature, error.hostid_ );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
        nResult = EXIT_FAILURE;
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
