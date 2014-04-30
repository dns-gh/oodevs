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
#include "tools/Main.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "tools/WinArguments.h"

namespace
{
    int Main( const tools::WinArguments& winArgs )
    {
        const bool silent = winArgs.HasOption( "--silent" );
        int argc = winArgs.Argc();
        auto argv = winArgs.Argv();
        gui::ApplicationMonitor monitor( argc, argv );
        // Set library path for Qt plugins
        monitor.addLibraryPath( QDir::currentPath() );
        try
        {
            ADN_App app( monitor, argc, argv );
            return app.Run();
        }
        catch( const std::exception& e )
        {
            MT_LOG_FATAL_ERROR_MSG( tools::GetStackTraceAndMessage( e ) );
            if( !silent )
                QMessageBox::critical( 0, "Critical", tools::GetExceptionMsg( e ).c_str() );
        }
        return EXIT_FAILURE;
    }
}

int main()
{
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    return tools::Main(
        tools::WinArguments( GetCommandLineW() ),
        MT_Logger_ABC::eAdaptation, false, &Main );
}
