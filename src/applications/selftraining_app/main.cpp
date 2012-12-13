// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Application.h"
#include "tools/Version.h"
#include "tools/WinArguments.h"
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"
#include "clients_gui/Tools.h"
#include <windows.h>

int main( int argc, char* argv[] )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword", 1.0f ) );
#endif

    BugTrap::Setup( tools::translate( "Application", "SWORD" ).toAscii().constData() )
            .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).toAscii().constData() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).toAscii().constData() );
    QApplication::setStyle( "windows" );
    QApplication::setDesktopSettingsAware( false );
    try
    {
        Application app( argc, argv );
        app.Initialize();
        app.exec();
    }
    catch( std::runtime_error& e )
    {
        QApplication app( argc, argv );
        QMessageBox::critical( 0, tools::translate( "Application", "Unhandled error" ), e.what() );
    }
    catch( std::exception& e )
    {
        QApplication app( argc, argv );
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
    }
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
