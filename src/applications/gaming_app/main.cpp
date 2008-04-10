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
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"
#include <qmessagebox.h>

namespace
{
    ////
    static const QString locale = "_en";
//    static const QString locale = "_fr";
    ////

    void PureHandler()
    {
        throw std::exception( "Pure virtual call" );
    }
}

int main( int argc, char** argv )
{   
    _set_purecall_handler( PureHandler );
    SetConsoleTitle( APP_NAME " - " APP_VERSION " - " __TIMESTAMP__ );

    std::auto_ptr< FlexLmLicense > license;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    license = FlexLmLicense::CheckLicense( "sword", 1.0f );
#endif

    BugTrap::Setup( "Sword Officer Training" )
            .SetEmail( "sword-ot@masagroup.net" )
            .SetVersion( APP_VERSION " - " __TIMESTAMP__ );

    Application app( argc, argv, locale, license.get() );
    try
    {
        app.Initialize();
        app.exec();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, APP_NAME " has crashed.", e.what() );
        throw;
    }

    return 0;
}