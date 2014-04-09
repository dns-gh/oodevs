// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Application.h"
#include "clients_gui/ApplicationMonitor.h"
#include "clients_kernel/Tools.h"
#include "tools/WinArguments.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include <extractor/Logging.h>
#include <gdal_ogr/GdalLogging.h>
#include <boost/scoped_ptr.hpp>

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
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::GetExceptionMsg( e ).c_str() );
    }
    catch( ... )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::translate( "Application", "Unhandled error" ) );
    }
    return EXIT_FAILURE;
}

namespace
{

int mainWrapper( int argc, char** argv )
{
    __try
    {
        return appMain( argc, argv );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
        return EXIT_FAILURE;
    }
}

}  // namespace

int main( int, char** )
{
    tools::WinArguments winArgs( GetCommandLineW() );
    boost::scoped_ptr< MT_FileLogger > logger;
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    logger.reset( new MT_FileLogger(
        debugDir / "preparation.log", 1, 0, MT_Logger_ABC::eLogLevel_All,
        false, MT_Logger_ABC::ePreparation ) );
    MT_LOG_REGISTER_LOGGER( *logger );
    MT_CrashHandler::SetRootDirectory( debugDir );
    extractor::SetLogger( CreateMTLogger( "extractor" ) );
    gdal_ogr::SetLogger( CreateMTLogger( "gdal_ogr" ) );

    int ret = mainWrapper( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
    if( logger )
        MT_LOG_UNREGISTER_LOGGER( *logger );
    return ret;
}
