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
#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"
#include "tools/Codec.h"
#include "tools/Main.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Exception.h>

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        try
        {
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
            license_gui::LicenseDialog::CheckLicense( "sword-replayer", !winArgs.HasOption( "verbose" ) );
#endif
            tools::SetCodec();
            HINSTANCE hInstance = GetModuleHandle( NULL );
            HINSTANCE prevInstance = GetModuleHandle( NULL );
            App app( hInstance, prevInstance, GetCommandLineW(), 0, !winArgs.HasOption( "--no-log" ) );
            app.Execute();
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}

int main()
{
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    return tools::Main(
        tools::WinArguments( GetCommandLineW() ),
        MT_Logger_ABC::eReplayer, true, &Main );
}
