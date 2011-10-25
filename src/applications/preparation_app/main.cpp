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

#if !defined( NO_LICENSE_CHECK )
#   include <tools/win32/FlexLm.h>
#endif

int main( int argc, char** argv )
{
    QString expiration;
#if !defined( NO_LICENSE_CHECK )
    const std::string licenseFeature = "sword-preparation";
    try
    {
        std::auto_ptr< FlexLmLicense > license = FlexLmLicense::CheckLicense( licenseFeature, 1.0f, "license.dat;.", FlexLmLicense::eCheckModeCustom );
        expiration = license->GetExpirationDate().c_str();
    }
    catch( FlexLmLicense::LicenseError& error )
    {
        license_gui::LicenseDialog::Run( licenseFeature, error.hostid_ );
        return 0;
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
            app.exec();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
        return 1;
    }
    catch( ... )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::translate( "Application", "Unhandled error" ) );
        return 1;
    }
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
