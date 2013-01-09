// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "clients_gui/ApplicationMonitor.h"
#include "ADN_App.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Version.h"
#include "tools/Version.h"

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

int main( int argc, char** argv )
{
    gui::ApplicationMonitor monitor( argc, argv );

#ifdef _DEBUG
    // Set library path for Qt plugins when debug because the repertory used is not the run dir
    monitor.setLibraryPaths( monitor.libraryPaths() << QDir::currentPath() );
#endif
    SetConsoleTitle( szADN_Version.c_str() );
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    const QStringList args = monitor.arguments();
    const bool silent = std::find( args.begin(), args.end(), "--silent" ) != args.end();
    const int nResultCode = RunAdn( monitor, argc, argv, silent );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResultCode;
}
