// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Application.h"
#include "license_gui/LicenseDialog.h"
#include "tools/WinArguments.h"
#include "tools/Version.h"
#include "tools/Win32/BugTrap.h"
#include "preparation/Tools.h"

//#define NO_LICENSE_CHECK

int main( int argc, char** argv )
{
    int nResult = EXIT_FAILURE;
    QString expiration;
#if !defined( NO_LICENSE_CHECK )
    try
    {
        std::string strExpiration;
        license_gui::LicenseDialog::CheckLicense( "sword-preparation", false, 0, &strExpiration );
        expiration = strExpiration.c_str();
    }
    catch( std::exception& /*e*/ )
    {
        return nResult;
    }
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

    BugTrap::Setup( tools::translate( "Application", "SWORD" ).ascii() )
            .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).ascii() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    try
    {
        if( app.Initialize() )
            nResult = app.exec();
        else
            nResult = EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
    }
    catch( ... )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::translate( "Application", "Unhandled error" ) );
    }
    return nResult;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
