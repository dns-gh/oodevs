// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "clients_gui/ApplicationMonitor.h"
#include "Application.h"
#include "tools/WinArguments.h"
#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
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

int main()
{
    tools::WinArguments winArgs( GetCommandLineW() ) ;

    // Change the current working directory, useful when combined with
    // --install and called by a registry key file type bindings. For deployed
    // applications, the expected cwd is almost always the parent applications
    // directory, but the option can also be used with development versions
    // from shell-impaired operating systems.
    tools::Path cwd = tools::Path::FromUTF8( winArgs.GetOption( "--cwd" ));
    if( !cwd.IsEmpty() && cwd.Exists() )
        boost::filesystem::current_path( cwd.ToBoost() );

    boost::scoped_ptr< MT_FileLogger > logger;
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    logger.reset( new MT_FileLogger(
        debugDir / "selftraining.log", 1, 0,
        MT_Logger_ABC::eLogLevel_All ) );
    MT_LOG_REGISTER_LOGGER( *logger );
    MT_CrashHandler::SetRootDirectory( debugDir );

    int ret = mainWrapper( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
    if( logger )
        MT_LOG_UNREGISTER_LOGGER( *logger );
    return ret;
}
