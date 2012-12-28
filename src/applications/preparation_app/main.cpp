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
#include "tools/WinArguments.h"
#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

namespace bfs = boost::filesystem;

namespace
{

bool GetOption( const tools::WinArguments& args, const std::string& name,
        std::string& value )
{
    const std::string n = name + "=";
    for( int i = 1; i < args.Argc(); ++i )
    {
        const std::string arg = args.Argv()[i];
        if( arg.find( n ) == 0 )
        {
            value = arg.substr( n.size() );
            return true;
        }
    }
    return false;
}

}  // namespace

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

int WINAPI WinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs( lpCmdLine ) ;
    std::string debugRoot;
    boost::scoped_ptr< MT_FileLogger > logger;
    if( GetOption( winArgs, "--debug-dir", debugRoot ))
    {
        bfs::path debugDir( debugRoot );
        bfs::create_directories( debugDir );
        boost::scoped_ptr< MT_FileLogger >( new MT_FileLogger(
            ( debugDir / "preparation.log" ).string().c_str(), 1, -1,
            MT_Logger_ABC::eLogLevel_All )).swap( logger );
        MT_LOG_REGISTER_LOGGER( *logger );
        MT_CrashHandler::SetRootDirectory( debugRoot );
    }

    int ret = mainWrapper( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ));
    if( logger )
        MT_LOG_UNREGISTER_LOGGER( *logger );
    return ret;
}
