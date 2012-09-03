// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Application.h"
#include "tools/WinArguments.h"
#include "tools/Version.h"
#include "tools/Win32/BugTrap.h"
#include "clients_kernel/Tools.h"

//#define NO_LICENSE_CHECK

#if !defined( NO_LICENSE_CHECK )
#   include <tools/win32/FlexLm.h>
#endif

int main( int argc, char** argv )
{
    QString expiration;
#if !defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license = FlexLmLicense::CheckLicense( "sword-preparation", 1.0f );
    expiration = license->GetExpirationDate().c_str();
#endif

    QApplication::setStyle( new QCleanlooksStyle );
    Application app( argc, argv, expiration );

    QFile file( "style.qss" );
    if( !file.open( QIODevice::Text | QFile::ReadOnly ) )
        QMessageBox::warning( 0, tools::translate( "Application", "Warning" ), "Style file missing. Loading default parameters." );
    else
    {
        app.setStyleSheet( file.readAll() );
        file.close();
    }

    BugTrap::Setup( tools::translate( "Application", "SWORD" ).toAscii().constData() )
            .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).toAscii().constData() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).toAscii().constData() );
    try
    {
        if( app.Initialize() )
            app.exec();
    }
    catch( std::exception& e )
    {
        MessageBox( 0, e.what(), "SWORD - Invalid input data", MB_ICONERROR | MB_OK | MB_TOPMOST );
        return 1;
    }
    catch( ... )
    {
        MessageBox( 0, "Unhandled error", "SWORD - Invalid input data", MB_ICONERROR | MB_OK | MB_TOPMOST );
        return 1;
    }
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
