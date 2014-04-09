// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"
#include "clients_gui/ApplicationMonitor.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Version.h"
#include "tools/VersionHelper.h"
#include "tools/WinArguments.h"

static const std::string szADN_Version   = "ADN - " + std::string( tools::AppProjectVersion() ) + " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;

namespace
{
    int RunAdn( gui::ApplicationMonitor& monitor, int argc, char** argv, bool silent )
    {
        try
        {
            ADN_App app( monitor, argc, argv );
            return app.Run();
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
            if( !silent )
                QMessageBox::critical( 0, "Critical", tools::GetExceptionMsg( e ).c_str() );
        }
        catch( ... )
        {
            const char* error = "Unhandled error";
            MT_LOG_ERROR_MSG( error );
            if( !silent )
                QMessageBox::critical( 0, "Critical", error );
        }
        return EXIT_FAILURE;
    }
}

int main()
{
    tools::WinArguments winArgs( GetCommandLineW() );
    int argc = winArgs.Argc();
    char** argv = const_cast< char** >( winArgs.Argv() );
    gui::ApplicationMonitor monitor( argc, argv );

    // Set library path for Qt plugins
    monitor.addLibraryPath( QDir::currentPath() );

    SetConsoleTitle( szADN_Version.c_str() );
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    const QStringList args = monitor.arguments();
    const bool silent = std::find( args.begin(), args.end(), "--silent" ) != args.end();
    const int nResultCode = RunAdn( monitor, argc, argv, silent );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResultCode;
}
