// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"
#include "ADN_Exception_ABC.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Version.h"
#include "tools/Version.h"

static const std::string szADN_Version   = "ADN - " + std::string( tools::AppProjectVersion() ) + " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;

int main( int argc, char** argv )
{
    SetConsoleTitle( szADN_Version.c_str() );
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int nResultCode = EXIT_FAILURE;
    bool isGUI = true;
    try
    {
        ADN_App app( argc, argv, "sword-authoring" );
        isGUI = app.GetOutputFile().empty();
        nResultCode = app.Run();
    }
    catch( ADN_Exception_ABC& e )
    {
        QMessageBox::critical( 0, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
    }
    catch( std::exception& exception )
    {
        if( isGUI )
            MessageBox( 0, exception.what(), "Sword Adaptation Tool - Exception", MB_ICONERROR | MB_OK );
        else
        {
            MT_LOG_ERROR_MSG( exception.what() );
            QMessageBox::critical( 0, "Critical", exception.what() );
        }
    }
    catch( ... )
    {
        QMessageBox::critical( 0, "Critical", "Unknown error !" );
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResultCode;
}

//#include "tools/WinArguments.h"
//int WINAPI WinMain( HINSTANCE /* hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
//{
//    tools::WinArguments winArgs(lpCmdLine) ;
//    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
//}
