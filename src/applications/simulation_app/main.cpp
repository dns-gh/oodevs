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
#include <tools/win32/CrashHandler.h>
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        std::unique_ptr< SIM_App > app;
        int result = EXIT_FAILURE;
        try
        {
            const bool verbose = winArgs.HasOption( "--verbose" );
            int maxConnections = 0;
    #if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
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
    #endif
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            HINSTANCE hInstance = GetModuleHandle( NULL );
            HINSTANCE prevInstance = GetModuleHandle( NULL );
            app.reset( new SIM_App( hInstance, prevInstance, GetCommandLineW(), 0, maxConnections, verbose ) );
            result = app->Execute();
        }
        catch( const FlexLmLicense::LicenseError& e )
        {
            MT_LOG_FATAL_ERROR_MSG( e.CreateLoggerMsg() );
        }
        catch( const xml::exception& e )
        {
            MT_LOG_FATAL_ERROR_MSG( tools::GetExceptionMsg( e ) );
        }
        catch( const tools::Exception& e )
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
