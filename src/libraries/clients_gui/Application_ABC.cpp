// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Application_ABC.h"
#include "VerticalHeaderTableView.h"
#include "clients_kernel/Tools.h"
#include "license_gui/LicenseDialog.h"
#include "tools/Codec.h"
#include "tools/Version.h"
#include "tools/Win32/BugTrap.h"

//#define NO_LICENSE_CHECK

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Application_ABC constructor
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
Application_ABC::Application_ABC( int& argc, char** argv )
    : QApplication( argc, argv )
    , invalidLicense_( true )
{
    SetLocale();
    tools::SetCodec();
}

// -----------------------------------------------------------------------------
// Name: Application_ABC destructor
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
Application_ABC::~Application_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::SetLocale
// Created: ABR 2012-07-13
// -----------------------------------------------------------------------------
void Application_ABC::SetLocale()
{
    locale_  = tools::readLocale();
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::Initialize
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application_ABC::Initialize()
{
    // Locale
    QLocale::setDefault( locale_ );

    // Translator
    CreateTranslators();

    // Post translator initialization
    InitializeBugTrap();
    InitializeStyle();
    InitializeLayoutDirection();
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::CheckLicense
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::CheckLicense( const std::string& licenseName )
{
    invalidLicense_ = false;
#if !defined( NO_LICENSE_CHECK )
    try
    {
        std::string strExpiration;
        license_gui::LicenseDialog::CheckLicense( licenseName, false, 0, &strExpiration );
        expiration_ = QString::fromStdString( strExpiration );
    }
    catch( std::exception& /*e*/ )
    {
        invalidLicense_ = true;
    }
#endif
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::InitializeBugTrap
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::InitializeBugTrap()
{
    BugTrap::Setup( tools::translate( "Application", "SWORD" ).toUtf8().constData() )
            .SetEmail( tools::translate( "Application", "sword@masagroup.net" ).toUtf8().constData() )
            .SetVersion( QString( "%1 - " __TIMESTAMP__ ).arg( tools::AppVersion() ).toUtf8().constData() );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::InitializeStyle
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::InitializeStyle()
{
    setStyle( new QCleanlooksStyle ); // $$$$ ABR 2012-07-12: Still needed ?
    setStyle( new gui::VerticalHeaderStyle( style() ) );

    QFile file( "style.qss" );
    if( !file.open( QIODevice::Text | QFile::ReadOnly ) )
        QMessageBox::warning( 0, tools::translate( "Application", "Warning" ), "Style file missing. Loading default parameters." );
    else
    {
        setStyleSheet( file.readAll () );
        file.close();
    }
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::InitializeLayoutDirection
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::InitializeLayoutDirection()
{
    setLayoutDirection( QApplication::translate( "Application", "QT_LAYOUT_DIRECTION" ) == "RTL" ? Qt::RightToLeft : Qt::LeftToRight );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::IsInvalidLicense
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
bool Application_ABC::IsInvalidLicense() const
{
    return invalidLicense_;
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::GetExpiration
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
const QString& Application_ABC::GetExpiration() const
{
    return expiration_;
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::AddTranslator
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
void Application_ABC::AddTranslator( const char* t )
{
    QTranslator* translator = tools::AddTranslator( *this, locale_, t );
    if( translator )
        translators_.push_back( translator );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::notify
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
bool Application_ABC::notify( QObject* pReceiver, QEvent* pEvent )
{
    try
    {
        return QApplication::notify( pReceiver, pEvent );
    }
    catch( std::exception& e )
    {
        DisplayError( e.what() );
    }
    catch( ... )
    {
        DisplayError( tools::translate( "Application", "Unknown exception caught" ) );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::DisplayError
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Application_ABC::DisplayError( const QString& text ) const
{
    static bool active = false;
    if( !active )
    {
        active = true;
        QMessageBox::critical( activeWindow(), tools::translate( "Application", "SWORD" ), text, "Ok" );
        active = false;
    }
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::DeleteTranslators
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application_ABC::DeleteTranslators()
{
    for( std::vector< QTranslator* >::const_iterator it = translators_.begin(); it != translators_.end(); ++it )
        removeTranslator( *it );
    translators_.clear();
}
