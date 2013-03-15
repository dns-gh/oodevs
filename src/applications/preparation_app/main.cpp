// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
#include <boost/scoped_ptr.hpp>

int main( int argc, char** argv )
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
        return main( argc, argv );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
        return EXIT_FAILURE;
    }
}

}  // namespace

int WINAPI wWinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */, LPWSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs( lpCmdLine ) ;
    tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir" ) );
    boost::scoped_ptr< MT_FileLogger > logger;
    if( !debugDir.IsEmpty() )
    {
        debugDir.CreateDirectories();
        boost::scoped_ptr< MT_FileLogger >( new MT_FileLogger(
            debugDir / "preparation.log", 1, -1,
            MT_Logger_ABC::eLogLevel_All )).swap( logger );
        MT_LOG_REGISTER_LOGGER( *logger );
        MT_CrashHandler::SetRootDirectory( debugDir );
    }

    int ret = mainWrapper( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ));
    if( logger )
        MT_LOG_UNREGISTER_LOGGER( *logger );
    return ret;
}
