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
#include "MainWindow.h"
#include "MessageDialog.h" 
#include "SessionTray.h" 
#include "TrayIcon.h"
#include "TrayMenu.h" 
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h" 
#include "tools/ProcessList.h"
#include <qsettings.h>
#include <qtextcodec.h>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication( argc, argv )
    , controllers_( new kernel::Controllers() )  
{
    const QString locale = ReadLang();
    AddTranslator( "qt", locale );
    AddTranslator( "frontend", locale );
    AddTranslator( "clients_kernel", locale );
    AddTranslator( "clients_gui", locale );
    AddTranslator( "selftraining_app", locale );
    AddTranslator( "tools", locale );

    // Child processe status 
    mainWindow_ = new MainWindow( *controllers_ );
    mainWindow_->show();
    setMainWidget( mainWindow_ );

    // check for previous instances running 
    tools::ProcessList processes;
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
// Name: Application destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
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
