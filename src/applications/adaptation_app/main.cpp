// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "clients_gui/ApplicationMonitor.h"
#include "ADN_App.h"
#include "ADN_Exception_ABC.h"
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
        catch( ADN_Exception_ABC& e )
        {
            MT_LOG_ERROR_MSG( e.GetExceptionMessage().c_str() );
            if( !silent )
                QMessageBox::critical( 0, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
        }
        catch( std::exception& exception )
        {
            MT_LOG_ERROR_MSG( exception.what() );
            if( !silent )
                QMessageBox::critical( 0, "Critical", exception.what() );
        }
        catch( ... )
        {
            const char* error = "Unknown error !";
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
    // Set library path for Qt plugins
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
