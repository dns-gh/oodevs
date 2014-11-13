// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Main.h"
#include "tools/VersionHelper.h"
#include "tools/WinArguments.h"
#include "simulation_terrain/TER_Logger.h"
#include <license_gui/LicenseDialog.h>
#include <tools/win32/FlexLm.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        std::unique_ptr< SIM_App > app;
        int result = EXIT_FAILURE;
        try
        {
            int maxConnections = 0;
    #if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
            const bool verbose = winArgs.HasOption( "--verbose" );
            license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
            try
            {
                FlexLmLicense license_dispatch( "sword-dispatcher", 1.0f );
                maxConnections = license_dispatch.GetAuthorisedUsers();
            }
            catch( const FlexLmLicense::LicenseError& )
            {
                maxConnections = 1;
            }
    #else
            (void)winArgs;
    #endif
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            app.reset( new SIM_App( maxConnections ) );
            result = app->Execute();
        }
        catch( const FlexLmLicense::LicenseError& e )
        {
            MT_LOG_FATAL_ERROR_MSG( e.CreateLoggerMsg() );
        }
        catch( const std::exception& e )
        {
            MT_LOG_FATAL_ERROR_MSG( tools::GetStackTraceAndMessage( e ) );
        }
        app.reset();
        google::protobuf::ShutdownProtobufLibrary();
        return result;
    }
}

int main()
{
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    InitializeTerrainLogger();
    return tools::Main(
        tools::WinArguments( GetCommandLineW() ),
        MT_Logger_ABC::eSimulation, true, &Main );
}
