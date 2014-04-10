// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/Main.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Path.h>
#include <tools/Exception.h>
#include <windows.h>

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        int result = EXIT_FAILURE;
        try
        {
            int maxConnections = 0;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
            // verbose mode
            const bool verbose = winArgs.HasOption( "--verbose" );
            // Check license
            license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
            license_gui::LicenseDialog::CheckLicense( "sword-dispatcher", !verbose, &maxConnections );
#endif
            // Execute dispatcher
            tools::SetCodec();
            Application app( winArgs.Argc(), winArgs.Argv(), maxConnections );
            result = app.Execute( winArgs.HasOption( "--test" ) );
        }
        catch( const std::exception& e )
        {
            MT_LOG_FATAL_ERROR_MSG( tools::GetExceptionMsg( e ) );
        }
        return result;
    }

}

int main()
{
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    return tools::Main(
        tools::WinArguments( GetCommandLineW() ),
        MT_Logger_ABC::eDispatcher, true, &Main );
}
