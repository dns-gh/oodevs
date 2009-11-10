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
#include "tools/Version.h"
#include "tools/WinArguments.h" 
#include "tools/Win32/BugTrap.h"
#include "gaming/Tools.h"
#include <qmessagebox.h>
#include <windows.h>

//#define NO_LICENSE_CHECK

#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
#   pragma warning( push )
#   pragma warning( disable: 4127 4512 )
#   include <boost/date_time/gregorian/gregorian.hpp>
#   pragma warning( pop )
#   include "tools/win32/FlexLm.h"
using namespace boost::gregorian;
#else
class FlexLmLicense {};
#endif

int main( int argc, char** argv )
{
    std::auto_ptr< FlexLmLicense > license;
    QString expiration;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    license = FlexLmLicense::CheckLicense( "sword", 1.0f );
    const boost::gregorian::date expirationDate( license->GetExpirationDate() );
    if( !expirationDate.is_infinity() )
        expiration = boost::gregorian::to_simple_string( expirationDate ).c_str();
#endif

    QApplication::setStyle( "windowsxp" );
    Application app( argc, argv, expiration );

    BugTrap::Setup( tools::translate( "Application", "SWORD" ).ascii() )
            .SetEmail( tools::translate( "Application", "sword@masagroup.net" ).ascii() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).ascii() );
    try
    {
        app.Initialize();
        app.exec();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "Unhandled error" ), e.what() );
        throw;
    }

    return 0;
}

int WINAPI WinMain( HINSTANCE /*hinstance */, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
{
    WinArguments winArgs(lpCmdLine) ; 
    return main( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) ); 
}
