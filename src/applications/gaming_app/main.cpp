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

int WINAPI wWinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */, LPWSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
