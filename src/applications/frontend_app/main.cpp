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
#include "tools/Win32/BugTrap.h"
#include "ENT/ENT_Tr.h"

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
        AddTranslator( "ENT" );
        AddTranslator( "clients_kernel" );
        AddTranslator( "clients_gui" );
        ENT_Tr::InitTranslations();
        app.exec();
    }
}

int main( int argc, char* argv[] )
{
    BugTrap::Setup( "Sword Officer Training" )
        .SetEmail( "sword-ot@masagroup.net" )
        .SetVersion( __TIMESTAMP__ );
    Run( argc, argv );
    return 0;
}
