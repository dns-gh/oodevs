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
#include "MainWindow.h"
#include "Config.h"
#include "ErrorEvent.h"
#include "FileLoaderObserver.h"
#include "preparation/StaticModel.h"
#include "preparation/Model.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Tr.h"
#include "preparation/Tools.h"
#include <qsettings.h>
#include <qtextcodec.h>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, const QString& license )
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
    staticModel_.reset( new StaticModel( *controllers_ ) );
    model_.reset( new Model( *controllers_, *staticModel_ ) );
    mainWindow_ = new MainWindow( *controllers_, *staticModel_, *model_, *config_, license_ );
    if( config_->HasGenerateScores() )
        return false;

    mainWindow_->show();

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

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
        if( ErrorEvent* pErrorEvent = dynamic_cast< ErrorEvent* >( pEvent ) )
        {
            DisplayError( pErrorEvent->reason_ );
            return true;
        }
        return QApplication::notify( pReceiver, pEvent );
    }
    catch( std::exception& e )
    {
        DisplayError( e.what() );
    }
    catch( ... )
    {
        DisplayError( tr("Unknown exception caught" ) );
    }
    return true;
}

namespace
{
    bool active = false;
}

// -----------------------------------------------------------------------------
// Name: Application::DisplayError
// Created: HBD 2010-11-08
// -----------------------------------------------------------------------------
void Application::DisplayError( const QString& text ) const
{
     if( ! active )
    {
        active = true;
        QMessageBox::critical( activeWindow(),  tools::translate( "Application", "SWORD" ), text, "Ok" );
        active = false;
     }
}
