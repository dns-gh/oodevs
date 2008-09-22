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
#include "tools/ProcessList.h"
#include "tools/ApplicationMutex.h" 
#include "clients_kernel/Tools.h" 
#include "TrayIcon.h"
#include "TrayMenu.h" 
#include "SessionStatus.h" 
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, const QString& locale )
    : QApplication ( argc, argv )
    , appMutex_( new tools::ApplicationMutex("SwordSelfTrainingApp") )
    , controllers_( new kernel::Controllers() )  

{

    AddTranslator( "qt", locale );
    AddTranslator( "frontend", locale );
    AddTranslator( "clients_kernel", locale );
    AddTranslator( "clients_gui", locale );
    AddTranslator( "selftraining_app", locale );    

    // Child processe status 
    boost::shared_ptr< SessionStatus > sessionStatus( new SessionStatus() ) ;  

    mainWindow_  = new MainWindow( *controllers_, sessionStatus ) ; 
    mainWindow_->show();
    setMainWidget( mainWindow_ ) ; 

    // check for previous instances running 
    tools::ProcessList processes ; 
    if (processes.Contains("selftraining_app.exe"))
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Already running" ), tools::translate( "Application", "The FrontEnd is already running. Close ?"  ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll("selftraining_app.exe"); 
        else
        {
            closeAllWindows(); 
            return ; 
        }
    }
    if (processes.Contains("simulation_app.exe"))
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Close Simulations" ), tools::translate( "Application", "Running Simulation(s) detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll("simulation_app.exe"); 
    }
    if (processes.Contains("gaming_app.exe"))
    {
        MessageDialog message( mainWindow_, tools::translate( "Application", "Close GUI" ), tools::translate( "Application", "Running GUI(s) detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
            processes.KillAll("gaming_app.exe"); 
    }

    trayMenu_.reset( new TrayMenu( mainWindow_, sessionStatus ) ) ;
    trayIcon_.reset( new TrayIcon( QPixmap( "resources/images/selftraining/csword.png" ), "Show window", trayMenu_.get() ) );
    QObject::connect(trayIcon_.get(), SIGNAL( clicked( const QPoint&) ), mainWindow_, SLOT( Maximize() ) );
    trayIcon_->show();
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Application::~Application()
{
    if ( trayIcon_.get() ) 
        trayIcon_->hide(); 
}

// -----------------------------------------------------------------------------
// Name: Application::AddTranslator
// Created: SBO 2008-04-09
// -----------------------------------------------------------------------------
void Application::AddTranslator( const char* t, const QString& locale )
{
    std::auto_ptr< QTranslator > trans( new QTranslator( this ) );
    if( trans->load( t + locale, "." ) || trans->load( t + locale, "resources/locales" ) )
       installTranslator( trans.release() );
}
