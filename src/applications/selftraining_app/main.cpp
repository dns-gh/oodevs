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
#include "license_gui/LicenseDialog.h"
#include "tools/Version.h"
#include "tools/WinArguments.h"
#include "tools/win32/FlexLm.h"
#include "tools/Win32/BugTrap.h"
#include "clients_gui/Tools.h"
#include <windows.h>

int main( int argc, char* argv[] )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    const std::string licenseFeature = "sword";
    try
    {
        std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( licenseFeature, 1.0f, "license.dat;.", FlexLmLicense::eCheckModeCustom ) );
    }
    catch( FlexLmLicense::LicenseError& error )
    {
        license_gui::LicenseDialog::Run( licenseFeature, error.hostid_ );
        return 0;
    }
#endif

    BugTrap::Setup( tools::translate( "Application", "SWORD" ).ascii() )
            .SetEmail( tools::translate( "Application", "sword-ot@masagroup.net" ).ascii() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    QApplication::setStyle( "windows" );
    QApplication::setDesktopSettingsAware( false );
    Application app( argc, argv );
    try
    {
        app.Initialize();
        app.exec();
    }
    catch( std::runtime_error& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Unhandled error" ), e.what() );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), e.what() );
    }
    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    tools::WinArguments winArgs(lpCmdLine) ;
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
}
