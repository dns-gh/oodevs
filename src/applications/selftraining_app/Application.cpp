// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Application.h"
#include "moc_Application.cpp"
#include "Config.h"
#include "ExerciseContainer.h"
#include "MainWindow.h"
#include "MessageDialog.h"
#include "ProgressPage.h"
#include "ReplayPage.h"
#include "SessionTray.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/LanguageController.h"
#include "clients_kernel/Tools.h"
#include "frontend/ProcessList.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/DefaultLoader.h"
#include "tools/Language.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::Application( gui::ApplicationMonitor& monitor, int argc, char** argv )
    : gui::Application_ABC( monitor )
    , waitCursor_( false )
{
    // Application_ABC initialization
    Initialize();

    // Data
    config_.reset( new Config() );
    config_->Parse( argc, argv );
    LoadCommandLineLanguage( config_->GetLanguages(), config_->GetCommandLineLanguage() );
    fileLoaderObserver_.reset( new tools::NullFileLoaderObserver() );
    fileLoader_.reset( new tools::DefaultLoader( *fileLoaderObserver_ ) );
    controllers_.reset( new kernel::Controllers() );
    exercises_.reset( new ExerciseContainer( *config_, controllers_->controller_ ));
    cursorTimer_.reset( new QTimer( this ) );
    connect( cursorTimer_.get(), SIGNAL( timeout() ), SLOT( OnWaitCursorTimeout() ) );

    // GUI
    mainWindow_.reset( new MainWindow( *this, *config_, *fileLoader_, *controllers_, *exercises_ ) );
    qApp->connect( qApp, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
    controllers_->languages_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::~Application()
{
    controllers_->languages_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Application::CreateTranslators
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void Application::CreateTranslators()
{
    AddTranslator( "qt" );
    AddTranslator( "frontend" );
    AddTranslator( "clients_kernel" );
    AddTranslator( "clients_gui" );
    AddTranslator( "selftraining_app" );
}

// -----------------------------------------------------------------------------
// Name: Application::InitializeStyle
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
void Application::InitializeStyle()
{
    app_.setStyle( "windows" );
    QApplication::setDesktopSettingsAware( false );
}

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
int Application::Run()
{
    const auto& exportReplay = config_->GetExportReplay();
    if( !exportReplay.IsEmpty() )
    {
        const auto error = ReplayPage::ExportReplay( mainWindow_.get(), *fileLoader_,
            *config_, config_->GetExercise(), config_->GetSession(), exportReplay );
        return !error.isEmpty();
    }
    mainWindow_->show();
    QCoreApplication::sendEvent( mainWindow_.get(), new QEvent( QEvent::LanguageChange ) );

    if( !config_->IsTestMode() )
    {
        // check for previous instances running
        frontend::ProcessList processes;
        if( processes.Contains( "selftraining_app.exe" ) )
        {
            MessageDialog message( mainWindow_.get(), tools::translate( "Application", "Already running" ), tools::translate( "Application", "The FrontEnd is already running. Start anyway ?" ), QMessageBox::Yes, QMessageBox::No );
            if( message.exec() == QMessageBox::No )
                return EXIT_SUCCESS;
        }
    }
    return app_.exec();
}

// -----------------------------------------------------------------------------
// Name: Application::SetWaitingCursor
// Created: JSR 2014-02-21
// -----------------------------------------------------------------------------
void Application::SetWaitingCursor()
{
    if( !waitCursor_ )
        qApp->setOverrideCursor( Qt::WaitCursor );
    waitCursor_ = true;
    cursorTimer_->start( 50 );
}

// -----------------------------------------------------------------------------
// Name: Application::OnWaitCursorTimeout
// Created: JSR 2014-02-21
// -----------------------------------------------------------------------------
void Application::OnWaitCursorTimeout()
{
    if( waitCursor_ )
        qApp->restoreOverrideCursor();
    waitCursor_ = false;
}

// -----------------------------------------------------------------------------
// Name: Application::GetMainWindow
// Created: JSR 2012-07-25
// -----------------------------------------------------------------------------
QWidget* Application::GetMainWindow()
{
    return mainWindow_.get();
}
