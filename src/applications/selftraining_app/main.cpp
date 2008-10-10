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
#include "gaming/Tools.h"

#include <windows.h>

namespace
{
    ////
//    static const QString locale = "_en";
    static const QString locale = "_fr";
    ////
}

int main( int argc, char* argv[] )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword", 1.0f ) );
#endif

    BugTrap::Setup( tools::translate( "Application", "SWORD Officer Training" ).ascii() )
            .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).ascii() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    QApplication::setStyle( "windows" );
    QApplication::setDesktopSettingsAware( false );
    Application app( argc, argv, locale );
    app.exec();
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    WinArguments winArgs(lpCmdLine) ; 
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) ); 
}