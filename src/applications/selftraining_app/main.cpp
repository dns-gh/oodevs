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
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"

namespace
{
    ////
    static const QString locale = "_en";
    ////

    void AddTranslator( const char* t )
    {
        QTranslator* trans = new QTranslator( qApp );
        trans->load( t + locale, "." );
        qApp->installTranslator( trans );
    }

    void Run( int argc, char** argv )
    {
        Application app( argc, argv );
        AddTranslator( "qt_" );
        app.exec();
    }
}

int main( int argc, char* argv[] )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword", 1.0f ) );
#endif

    BugTrap::Setup( "Sword Officer Training" )
        .SetEmail( "sword-ot@masagroup.net" )
        .SetVersion( __TIMESTAMP__ );
    Run( argc, argv );
    return 0;
}
