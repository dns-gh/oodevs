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
#include "Config.h"
#include "FileLoaderObserver.h"
#include "MainWindow.h"
#include "clients_kernel/Controllers.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/ModeController.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( int& argc, char** argv )
    : Application_ABC()
{
    // License
    CheckLicense( "sword-preparation" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // Data
    observer_.reset( new FileLoaderObserver() );
    config_.reset( new Config( argc, argv, *observer_ ) );
    controllers_.reset( new kernel::Controllers() );
    controllers_->SetModeController( new ::ModeController() );
    staticModel_.reset( new StaticModel( *controllers_ ) );
    model_.reset( new Model( *controllers_, *staticModel_ ) );

    // GUI
    mainWindow_ = new MainWindow( *controllers_, *staticModel_, *model_, *config_, GetExpiration() );
    qApp->connect( qApp, SIGNAL( lastWindowClosed() ), SLOT( quit() ) ); // Make sure that once the last window is closed, the application quits.
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
// Name: Application::CreateTranslators
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application::CreateTranslators()
{
    AddTranslator( "qt" );
    AddTranslator( "ENT" );
    AddTranslator( "actions" );
    AddTranslator( "actions_gui" );
    AddTranslator( "clients_kernel" );
    AddTranslator( "clients_gui" );
    AddTranslator( "clients_gui_app6" );
    AddTranslator( "indicators" );
    AddTranslator( "preparation" );
    AddTranslator( "preparation_app" );
    AddTranslator( "reports" );
    AddTranslator( "resources_gradients" );
    ENT_Tr::InitTranslations();
}

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
int Application::Run()
{
    if( IsInvalidLicense() )
        return EXIT_FAILURE;

    if( config_->HasGenerateScores() || !config_->GetFolderToMigrate().empty() )
        return EXIT_SUCCESS;

    mainWindow_->show();
    observer_->DisplayErrors();
    return qApp->exec();
}
