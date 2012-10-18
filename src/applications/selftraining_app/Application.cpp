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
#include "Launcher.h"
#include "MainWindow.h"
#include "MessageDialog.h"
#include "ProgressPage.h"
#include "SessionTray.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "frontend/LauncherClient.h"
#include "frontend/ProcessList.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/DefaultLoader.h"
#include "tools/Win32/BugTrap.h"
#include "license_gui/LicenseDialog.h"
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : gui::Application_ABC( argc, argv )
{
    // License
    CheckLicense( "sword" );
    if( IsInvalidLicense() )
        return;

    // Application_ABC initialization
    Initialize();

    // Data
    config_.reset( new Config() );
    config_->Parse( qApp->argc(), qApp->argv() );
    fileLoaderObserver_.reset( new tools::NullFileLoaderObserver() );
    fileLoader_.reset( new tools::DefaultLoader( *fileLoaderObserver_ ) );
    controllers_.reset( new kernel::Controllers() );
    launcher_.reset( new Launcher( argc, argv ) );
    launcherClient_.reset( new frontend::LauncherClient( controllers_->controller_ ) );
    timer_.reset( new QTimer( this ) );
    connect( timer_.get(), SIGNAL( timeout() ), SLOT( OnTimer() ) );

    // GUI
    mainWindow_ = new MainWindow( *config_, *fileLoader_, *controllers_, *launcherClient_ );
    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::~Application()
{
    launcher_.reset();
    if( timer_.get() )
        timer_->stop();
    launcherClient_.reset();
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
    QApplication::setStyle( "windows" );
    QApplication::setDesktopSettingsAware( false );
}

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
int Application::Run()
{
    if( IsInvalidLicense() )
        return EXIT_FAILURE;
    
    if( !launcher_->IsInitialized() )
    {
        QMessageBox::critical( mainWindow_, tools::translate( "Application", "Error" ), tools::translate( "Application", "Launcher service cannot be started: %1."  ).arg( launcher_->GetLastError().c_str() ) );
        return EXIT_FAILURE;
    }

    mainWindow_->show();
    QCoreApplication::sendEvent( mainWindow_, new QEvent( QEvent::LanguageChange ) );
    timer_->start( 10 );

    // check for previous instances running
    frontend::ProcessList processes;
    if( processes.Contains( "selftraining_app.exe" ) )
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Already running" ), tools::translate( "Application", "The FrontEnd is already running. Start anyway ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::No )
            return EXIT_SUCCESS;
    }
    if( processes.Contains( "simulation_app.exe" ) )
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Close Simulations" ), tools::translate( "Application", "Running Simulation(s) detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll( "simulation_app.exe" );
    }
    if( processes.Contains( "gaming_app.exe" ) )
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Close GUI" ), tools::translate( "Application", "Running GUI(s) detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll( "gaming_app.exe" );
    }

    return exec();
}

// -----------------------------------------------------------------------------
// Name: Application::OnTimer
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void Application::OnTimer()
{
    launcherClient_->Update();
}

// -----------------------------------------------------------------------------
// Name: Application::GetLauncher
// Created: ABR 2012-07-12
// -----------------------------------------------------------------------------
Launcher& Application::GetLauncher() const
{
    assert( launcher_.get() );
    return *launcher_;
}

// -----------------------------------------------------------------------------
// Name: Application::GetMainWindow
// Created: JSR 2012-07-25
// -----------------------------------------------------------------------------
QWidget* Application::GetMainWindow()
{
    return mainWindow_;
}

// -----------------------------------------------------------------------------
// Name: Application::notify
// Created: SBO 2011-03-24
// -----------------------------------------------------------------------------
bool Application::notify( QObject* emitter, QEvent* event )
{
    if( event && event->type() == QEvent::User + 666 )
        if( QString* message = static_cast< QString* >( static_cast< QCustomEvent* >( event )->data() ) )
        {
            QMessageBox::critical( 0, tools::translate( "Application", "Error" ), *message );
            delete message;
            return true;
        }
    if( event && event->type() == QEvent::User + 667 )
    {
        static_cast< ProgressPage* >( static_cast< QCustomEvent* >( event )->data() )->DoNotifyStopped();
        return true;
    }
    return QApplication::notify( emitter, event );
}
