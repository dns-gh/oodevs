// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Codec.h"
#include "tools/WinArguments.h"
#include <license_gui/LicenseDialog.h>
#include <tools/Path.h>
#include <tools/Exception.h>
#include <tools/win32/CrashHandler.h>
#include <windows.h>
#include <boost/smart_ptr.hpp>

namespace
{

void CrashHandler( EXCEPTION_POINTERS* exception )
{
    MT_CrashHandler::ExecuteHandler( exception );
}

}  // namespace

//-----------------------------------------------------------------------------
// Name: main constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int main( int /*argc*/, char* /*argv*/[] )
{
    // Init logger
    tools::WinArguments winArgs( GetCommandLineW() );
    MT_ConsoleLogger consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    boost::scoped_ptr< MT_FileLogger > fileLogger;
    const tools::Path debugDir = tools::Path::FromUTF8( winArgs.GetOption( "--debug-dir", "./Debug" ) );
    debugDir.CreateDirectories();
    MT_CrashHandler::SetRootDirectory( debugDir );
    fileLogger.reset( new MT_FileLogger( debugDir / "dispatcher.log", 1, 0,
        MT_Logger_ABC::eLogLevel_All ) );
    MT_LOG_REGISTER_LOGGER( *fileLogger );
    tools::InitPureCallHandler();
    tools::InitCrashHandler( &CrashHandler );

    int nResult = EXIT_FAILURE;
    try
    {
        int maxConnections = 0;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
        // verbose mode
        const bool verbose = winArgs.HasOption( "--verbose" );
        // Check license
        license_gui::LicenseDialog::CheckLicense( "sword-runtime", !verbose );
        license_gui::LicenseDialog::CheckLicense( "sword-dispatcher", !verbose, &maxConnections );
#endif

        // Execute dispatcher
        tools::SetCodec();
        Application app( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ), maxConnections );
        nResult = app.Execute( winArgs.HasOption( "--test" ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }

    if( fileLogger )
        MT_LOG_UNREGISTER_LOGGER( *fileLogger );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
