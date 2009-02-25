// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Application.h"
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"
#include "tools/Version.h"
#include "clients_gui/Tools.h"

int main( int argc, char* argv[] )
{
    QApplication::setStyle( "windowsxp" );
    Application app( argc, argv );

    #if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword", 1.0f ) );
    #endif

    BugTrap::Setup( tools::translate( "Application", "SWORD Officer Training" ).ascii() )
        .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).ascii() )
        .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    app.exec();
    return 0;
}
