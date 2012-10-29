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
#include "tools/WinArguments.h"
#include "clients_kernel/Tools.h"

int main( int argc, char** argv )
{
    QApplication qapp( argc, argv );
    try
    {
        Application app( argc, argv );
        return app.Run();
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
    }
    catch( ... )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::translate( "Application", "Unhandled error" ) );
    }
    return EXIT_FAILURE;
}

int WINAPI WinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
