// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Application.h"
#include "moc_Application.cpp"
#include "Config.h"
#include "Launcher.h"
#include "MainWindow.h"
#include "MessageDialog.h"
#include "SessionTray.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "frontend/LauncherClient.h"
#include "frontend/ProcessList.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/DefaultLoader.h"
#include <xeumeuleu/xml.hpp>
#include <qsettings.h>
#include <qtextcodec.h>
#include <qtimer.h>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
    std::auto_ptr< Config > GetConfig()
    {
        std::auto_ptr< Config > config( new Config() );
        config->Parse( qApp->argc(), qApp->argv() );
        return config;
    }
}

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication( argc, argv )
    , config_     ( GetConfig() )
    , fileLoaderObserver_( new tools::NullFileLoaderObserver() )
    , fileLoader_( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , controllers_( new kernel::Controllers() )
    , launcher_( new Launcher( argc, argv ) )
    , launcherClient_( new frontend::LauncherClient( controllers_->controller_ ) )
    , mainWindow_( 0 )
    , timer_( new QTimer( this ) )
{
    CreateTranslators();
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::~Application()
{
    launcher_.reset();
    timer_->stop();
    launcherClient_.reset();
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
void Application::Initialize()
{
    if( ! launcher_->IsInitialized() )
    {
        QMessageBox::critical( mainWindow_, tools::translate( "Application", "Error" ), tools::translate( "Application", "Launcher service cannot be started: %1."  ).arg( launcher_->GetLastError().c_str() ) );
        closeAllWindows();
    }

    mainWindow_ = new MainWindow( *config_, *fileLoader_, *controllers_, *launcherClient_ );
    setMainWidget( mainWindow_ );
    mainWindow_->show();

    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
    connect( timer_, SIGNAL( timeout() ), SLOT( OnTimer() ) );
    timer_->start( 10 );

    // check for previous instances running
    frontend::ProcessList processes;
    if( processes.Contains( "selftraining_app.exe" ) )
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Already running" ), tools::translate( "Application", "The FrontEnd is already running. Close ?"  ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll( "selftraining_app.exe" );
        else
        {
            closeAllWindows();
            return;
        }
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
    sessionTray_.reset( new SessionTray( mainWindow_ ) );
}

// -----------------------------------------------------------------------------
// Name: Application::CreateTranslators
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void Application::CreateTranslators()
{
    const QString locale = ReadLang();
    AddTranslator( "qt", locale );
    AddTranslator( "frontend", locale );
    AddTranslator( "clients_kernel", locale );
    AddTranslator( "clients_gui", locale );
    AddTranslator( "selftraining_app", locale );
    AddTranslator( "clients_gui_sword", locale );
}

// -----------------------------------------------------------------------------
// Name: Application::AddTranslator
// Created: SBO 2008-04-09
// -----------------------------------------------------------------------------
void Application::AddTranslator( const char* t, const QString& locale )
{
    std::auto_ptr< QTranslator > trans( new QTranslator( this ) );
    const QString file = QString( "%1_%2" ).arg( t ).arg( locale );
    if( trans->load( file, "." ) || trans->load( file, "resources/locales" ) )
       installTranslator( trans.release() );
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
    return QApplication::notify( emitter, event );
}
