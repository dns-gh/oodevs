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
#include "preparation/StaticModel.h"
#include "preparation/Model.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Workers.h"
#include "ENT/ENT_Tr.h"
#include "preparation/Tools.h"

using namespace kernel;

namespace 
{
    struct CatchMeIfYouCan {};
}

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, const QString& locale, const QString& license )
    : QApplication  ( argc, argv )
    , mainWindow_( 0 )
    , license_( license )
{
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "clients_kernel" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "preparation" );
    AddTranslator( locale, "preparation_app" );
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
// Name: Application::AddTranslator
// Created: AGE 2007-12-18
// -----------------------------------------------------------------------------
void Application::AddTranslator( const QString& locale, const char* t )
{
    std::auto_ptr< QTranslator > trans( new QTranslator( this ) );
    if( trans->load( t + locale, "." ) || trans->load( t + locale, "resources/locales" ) )
       installTranslator( trans.release() );
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
        QMessageBox::critical( 0, tools::translate( "Application", "SWORD Officer Training" ), e.what() );
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
    controllers_ = new Controllers();
    workers_     = new Workers();
    staticModel_ = new StaticModel( *controllers_ );
    model_       = new Model( *controllers_, *staticModel_ );
    mainWindow_  = new MainWindow( *controllers_, *staticModel_, *model_, *config_, license_ );
    mainWindow_->show();

    // Make sure the application exits when the main window is closed.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
}
