// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Application.h"
#include "clients_gui/ApplicationMonitor.h"
#include "clients_kernel/Tools.h"
#include "tools/WinArguments.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include <gdal_ogr/GdalLogging.h>
#include <tools/win32/CrashHandler.h>
#include <boost/scoped_ptr.hpp>

namespace
{

int appMain( int argc, char** argv )
{
    gui::ApplicationMonitor monitor( argc, argv );
    try
    {
        Application app( monitor, argc, argv );
        return app.Run();
    }
    catch( const std::exception& e )
    {
        const std::string err = tools::GetExceptionMsg( e );
        MT_LOG_ERROR_MSG( err );
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), err.c_str() );
    }
    return EXIT_FAILURE;
}

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

}  // namespace

int main( int, char** )
{
    tools::WinArguments winArgs( GetCommandLineW() ) ;
    tools::InitPureCallHandler();
    boost::scoped_ptr< MT_FileLogger > logger;
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    logger.reset( new MT_FileLogger(
        debugDir / "gaming.log", 1, 0, MT_Logger_ABC::eLogLevel_All,
        false, MT_Logger_ABC::eGaming ) );
    MT_LOG_REGISTER_LOGGER( *logger );
    MT_CrashHandler::SetRootDirectory( debugDir );
    tools::InitCrashHandler( &CrashHandler );
    gdal_ogr::SetLogger( CreateMTLogger( "gdal_ogr" ) );

    int ret = appMain( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
    if( logger )
        MT_LOG_UNREGISTER_LOGGER( *logger );
    return ret;
}
