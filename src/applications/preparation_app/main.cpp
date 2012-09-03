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
    int nResultCode = EXIT_FAILURE;
    try
    {
        Application app( argc, argv );
        nResultCode = app.Run();
    }
    catch( std::exception& e )
    {
        MessageBox( 0, e.what(), "SWORD - Invalid input data", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    catch( ... )
    {
        MessageBox( 0, "Unhandled error", "SWORD - Invalid input data", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    return nResultCode;
}

int WINAPI WinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
