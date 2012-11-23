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
#include "Config.h"
#include "ErrorEvent.h"
#include "FileLoaderObserver.h"
#include "MainWindow.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/ModeController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/VerticalHeaderTableView.h"
#include "ENT/ENT_Tr.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace
{
    QString ReadLang()
    {
        QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }

    bool TransformLang( std::string& language )
    {
        if( language != "fr" && language != "en" && language != "es" )
        {
            language = boost::algorithm::to_lower_copy( language );
            if( language.find( "fr" ) != std::string::npos )
                language = "fr";
            else if( language.find( "es" ) != std::string::npos )
                language = "es";
            else
                language = "en";
            return true;
        }
        return false;
    }

    class Locale : public QLocale
    {
    public:
        Locale( const std::string& locale )
            : QLocale( locale == "en" ? QLocale::English : locale == "fr" ? QLocale::French : QLocale::Spanish, locale == "en" ? QLocale::UnitedStates : locale == "fr" ? QLocale::France: QLocale::Spain )
        {
            // NOTHING
        }

        virtual ~Locale(){}
    };
}

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( int& argc, char** argv, const QString& license )
    : Application_ABC( argc, argv )
    , mainWindow_( 0 )
    , license_( license )
{
    const QString locale = ReadLang();
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "actions" );
    AddTranslator( locale, "actions_gui" );
    AddTranslator( locale, "clients_kernel" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "indicators" );
    AddTranslator( locale, "preparation" );
    AddTranslator( locale, "preparation_app" );
    AddTranslator( locale, "clients_gui_app6" );
    AddTranslator( locale, "clients_gui_sword" );
    AddTranslator( locale, "resources_gradients" );
    ENT_Tr::InitTranslations();
    setStyle( new gui::VerticalHeaderStyle( style() ) );

    std::string localeStr = locale.toAscii().constData();
    TransformLang( localeStr );
    QLocale::setDefault( Locale( localeStr ) );
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
bool Application::Initialize()
{
    return Initialize( argc(), argv() );
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
bool Application::Initialize( int argc, char** argv )
{
    observer_.reset( new FileLoaderObserver() );
    config_.reset( new Config( argc, argv, *observer_ ) );
    controllers_.reset( new kernel::Controllers() );
    controllers_->SetModeController( new ::ModeController() );
    staticModel_.reset( new StaticModel( *controllers_ ) );
    model_.reset( new Model( *controllers_, *staticModel_ ) );
    mainWindow_ = new MainWindow( *controllers_, *staticModel_, *model_, *config_, license_ );
    if( config_->HasGenerateScores() || !config_->GetFolderToMigrate().empty() )
        return false;

    mainWindow_->show();
    observer_->DisplayErrors();

    return true;
}

// -----------------------------------------------------------------------------
// Name: Application::notify
// Created: HBD 2010-11-08
// -----------------------------------------------------------------------------
bool Application::notify( QObject* pReceiver, QEvent* pEvent )
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
        DisplayError( tr( "Unknown exception caught" ) );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Application::DisplayError
// Created: HBD 2010-11-08
// -----------------------------------------------------------------------------
void Application::DisplayError( const QString& text ) const
{
    static bool active = false;
    if( ! active )
    {
        active = true;
        QMessageBox::critical( activeWindow(),  tools::translate( "Application", "SWORD" ), text, "Ok" );
        active = false;
    }
}
