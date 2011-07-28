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
#include "tools/Version.h"
#include "tools/WinArguments.h"
#include "tools/Win32/BugTrap.h"
#include "gaming/Tools.h"
#include <qmessagebox.h>
#include <windows.h>

//#define NO_LICENSE_CHECK

#if !defined( NO_LICENSE_CHECK )
#   include <tools/win32/FlexLm.h>
#endif

int main( int argc, char** argv )
{
    QString expiration;
#if !defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > pLicense = FlexLmLicense::CheckLicense( "sword-gaming", 1.0f );
    expiration = pLicense->GetExpirationDate().c_str();
#endif

    QApplication::setStyle( "windowsxp" );
    Application app( argc, argv, expiration );
    BugTrap::Setup( tools::translate( "Application", "SWORD" ).ascii() )
            .SetEmail( tools::translate( "Application", "sword@masagroup.net" ).ascii() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    try
    {
        app.Initialize();
        app.exec();
    }
    catch( std::runtime_error& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Unhandled error" ), e.what() );
        return 1;
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
        return 1;
    }
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
