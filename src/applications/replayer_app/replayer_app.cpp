// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/IpcWatch.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Exception.h>
#include <tools/win32/CrashHandler.h>
#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <windows.h>

namespace
{

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

} // namespace

int main( int /*argc*/, char* /*argv*/[] )
{
    tools::WinArguments winArgs( GetCommandLineW() );
    tools::InitPureCallHandler();
    boost::scoped_ptr< MT_FileLogger > fileLogger;
    tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "" ));
    if( !debugDir.IsEmpty() )
    {
        debugDir.CreateDirectories();
        fileLogger.reset( new MT_FileLogger( debugDir / "replayer.log", 1, -1,
            MT_Logger_ABC::eLogLevel_All ) );
        MT_LOG_REGISTER_LOGGER( *fileLogger );
    }
    else
    {
        debugDir = tools::Path::FromUTF8( "./Debug" );
    }

    try
    {
        debugDir.CreateDirectories();
        MT_CrashHandler::SetRootDirectory( debugDir );
        tools::InitCrashHandler( &CrashHandler );
    }
    catch( std::exception& )
    {
        // NOTHING
    }

    int nResult = EXIT_SUCCESS;
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    try
    {
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        license_gui::LicenseDialog::CheckLicense( "sword-replayer", !winArgs.HasOption( "verbose" ) );
#endif
        tools::SetCodec();

        //initialising replayer_app
        MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
        MT_LOG_STARTUP_MESSAGE( "Sword(tm) Replayer" );
        MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

        tools::RealFileLoaderObserver_ABC* observer = new tools::NullFileLoaderObserver();
        dispatcher::Config* config = new dispatcher::Config( *observer );
        tools::WaitEvent* quit = new tools::WaitEvent();

        // win32 argument parsing
        bool test = winArgs.HasOption( "--test" );
        config->Parse( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
        if( !winArgs.HasOption( "--no-log" ) )
            MT_LOG_REGISTER_LOGGER( *new MT_FileLogger( config->BuildSessionChildFile( "Replayer.log" ), 1, -1, MT_Logger_ABC::eLogLevel_All, true ) );

        MT_LOG_INFO_MSG( "Loading record " << config->GetSessionFile() );

        std::auto_ptr< dispatcher::Replayer > replayer;
        replayer.reset( new dispatcher::Replayer( *config ) );

        //execute replayer_app
        try
        {
            tools::ipc::Watch watch( *quit );
            do
            replayer->Update();
            while( !test && !quit->Wait( boost::posix_time::milliseconds( 10 ) ) );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Replayer error : " << tools::GetExceptionMsg( e ) );
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        nResult = EXIT_FAILURE;
    }
    if( fileLogger )
        MT_LOG_UNREGISTER_LOGGER( *fileLogger );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
