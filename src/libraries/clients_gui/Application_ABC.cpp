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
#include "moc_Application_ABC.cpp"
#include "ApplicationMonitor.h"
#include "VerticalHeaderTableView.h"
#include "clients_kernel/Tools.h"
#include "tools/Codec.h"
#include "tools/Language.h"
#include "tools/Languages.h"
#include "tools/VersionHelper.h"
#include <license_gui/LicenseDialog.h>

//#define NO_LICENSE_CHECK

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Application_ABC constructor
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
Application_ABC::Application_ABC( ApplicationMonitor& monitor )
    : app_           ( monitor )
    , invalidLicense_( true )
{
    connect( &app_, SIGNAL( Error( const QString& ) ), SLOT( DisplayError( const QString& ) ) );
    QLocale::setDefault( QLocale::system() );
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
// Name: Application_ABC::Initialize
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application_ABC::Initialize()
{
    // Translator
    tools::Language::InitFromRegistry();
    CreateTranslators();

    // Post translator initialization
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
    catch( const std::exception& /*e*/ )
    {
        invalidLicense_ = true;
    }
#endif
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::InitializeStyle
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::InitializeStyle()
{
    app_.setStyle( "cleanlooks" );
    QFile file( "style.qss" );
    if( !file.open( QIODevice::Text | QFile::ReadOnly ) )
        QMessageBox::warning( 0, tools::translate( "Application", "Warning" ), "Style file missing. Loading default parameters." );
    else
    {
        app_.setStyleSheet( file.readAll () );
        file.close();
    }
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::InitializeLayoutDirection
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
void Application_ABC::InitializeLayoutDirection()
{
    app_.setLayoutDirection( QApplication::translate( "Application", "QT_LAYOUT_DIRECTION" ) == "RTL" ? Qt::RightToLeft : Qt::LeftToRight );
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
void Application_ABC::AddTranslator( const std::string& t )
{
    AddTranslator( t, tools::Language::Current() );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::AddTranslator
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
void Application_ABC::AddTranslator( const std::string& t, const std::string& languageCode )
{
    QTranslator* translator = tools::AddTranslator( app_, languageCode, t.c_str() );
    if( translator )
        translators_[ t ] = translator;
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
        QMessageBox::critical( app_.activeWindow(), tools::translate( "Application", "SWORD" ), text, "Ok" );
        active = false;
    }
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::DeleteTranslators
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application_ABC::DeleteTranslators()
{
    for( auto it = translators_.begin(); it != translators_.end(); ++it )
        app_.removeTranslator( it->second );
    translators_.clear();
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::DeleteTranslators
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
void Application_ABC::DeleteTranslator( const std::string& translator )
{
    const auto it = translators_.find( translator );
    if( it != translators_.end() )
    {
        app_.removeTranslator( it->second );
        translators_.erase( it );
    }
}

namespace
{

    void CheckNamingHierarchy( std::ostream& out, QObject* parent, std::map< std::string, std::pair< unsigned int, unsigned int > >& map, const QString& parentPath )
    {
        if( !parent )
            return;
        for( auto it  = parent->children().begin(); it != parent->children().end(); ++it )
        {
            std::string className = ( *it )->metaObject()->className();
            QString path = parentPath + "." + className.c_str();
            bool hasName =  !( *it )->objectName().isEmpty();
            out << hasName << "," << path << "," << parent->objectName() << std::endl;
            if( hasName )
                ++map[ className ].first;
            else
                ++map[ className ].second;

            CheckNamingHierarchy( out, *it, map, path );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Application::CheckInterfaceComponentNaming
// Created: NPT 2013-03-21
// -----------------------------------------------------------------------------
void Application_ABC::CheckInterfaceComponentNaming( QObject* root, const tools::Path& outpath ) const
{
    std::ostream* output = &std::cout;
    std::fstream fp;
    if( outpath != "-" )
    {
        fp.open( outpath.ToUnicode(), std::ios::out );
        output = &fp;
    }
    std::map< std::string, std::pair< unsigned int, unsigned int > > map;
    CheckNamingHierarchy( *output, root, map, "" );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::OnLanguageChanged
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
void Application_ABC::OnLanguageChanged()
{
    DeleteTranslators();
    CreateTranslators();
    InitializeLayoutDirection();
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::LoadCommandLineLanguage
// Created: ABR 2013-11-28
// -----------------------------------------------------------------------------
void Application_ABC::LoadCommandLineLanguage( const tools::Languages& languages, const std::string& languageCode )
{
    if( languageCode.empty() )
    {
        // If no command line language, ensure registry language is supported
        if( !languages.EnsureCurrentIsSupported() )
            // if it's not supported, fall back to English so reload translators
            OnLanguageChanged();
        return;
    }
    tools::Language::SetCurrent( languageCode );
    languages.EnsureCurrentIsSupported();
    OnLanguageChanged();
}
