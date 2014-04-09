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
#include "tools/Main.h"
#include "tools/WinArguments.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_Logger.h"
#include <gdal_ogr/GdalLogging.h>
#include <tools/win32/CrashHandler.h>

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

int main()
{
    tools::InitPureCallHandler();
    const tools::WinArguments winArgs( GetCommandLineW() ) ;
    const auto logger = tools::Initialize( winArgs, MT_Logger_ABC::eGaming );
    tools::InitCrashHandler( &CrashHandler );
    gdal_ogr::SetLogger( CreateMTLogger( "gdal_ogr" ) );
    return appMain( winArgs.Argc(), const_cast< char** >( winArgs.Argv() ) );
}
