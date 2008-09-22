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
#include "tools/WinArguments.h"
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"

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

    BugTrap::Setup( "Sword Officer Training" )
        .SetEmail( "sword-ot@masagroup.net" )
        .SetVersion( __TIMESTAMP__ );
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