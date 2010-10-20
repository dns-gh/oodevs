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
#include "MainWindow.h"
#include "Config.h"
#include "preparation/StaticModel.h"
#include "preparation/Model.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Tr.h"
#include "preparation/Tools.h"
#include <qsettings.h>
#include <qtextcodec.h>

namespace
{
    struct CatchMeIfYouCan {};

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
    AddTranslator( locale, "clients_gui_sword" );
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
void Application::Initialize()
{
    try
    {
        Initialize( argc(), argv() );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( 0, tools::translate( "Application", "SWORD" ), e.what() );
        throw CatchMeIfYouCan();
    }
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::Initialize( int argc, char** argv )
{
    config_      = new Config( argc, argv );
    controllers_ = new kernel::Controllers();
    staticModel_ = new StaticModel( *controllers_ );
    model_       = new Model( *controllers_, *staticModel_ );
    mainWindow_  = new MainWindow( *controllers_, *staticModel_, *model_, *config_, license_ );
    mainWindow_->show();

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
}
