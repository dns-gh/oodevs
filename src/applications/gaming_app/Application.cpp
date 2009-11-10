// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Application.h"
#include "moc_Application.cpp"

#include "MainWindow.h"
#include "Config.h"
#include "LoggerProxy.h"
#include "RcEntityResolver.h"
#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Model.h"
#include "gaming/Services.h"
#include "gaming/Simulation.h"
#include "gaming/Profile.h"
#include "gaming/CommandHandler.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Workers.h"
#include "gaming/AgentServerMsgMgr.h"
#include "ENT/ENT_Tr.h"
#include <qsettings.h>
#include <qtextcodec.h>

using namespace kernel;

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
Application::Application( int argc, char** argv, const QString& expiration )
    : QApplication( argc, argv )
    , mainWindow_ ( 0 )
    , expiration_ ( expiration )
{
    const QString locale = ReadLang();
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "clients_kernel" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "gaming" );
    AddTranslator( locale, "actions" );
    AddTranslator( locale, "gaming_app" );
    AddTranslator( locale, "tools" );
    ENT_Tr::InitTranslations();
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::~Application()
{
    delete network_;
    delete workers_;
    delete config_;
}

// -----------------------------------------------------------------------------
// Name: Application::AddTranslator
// Created: AGE 2007-12-18
// -----------------------------------------------------------------------------
void Application::AddTranslator( const QString& locale, const char* t )
{
    QTranslator* trans = new QTranslator( this );
    const QString file = QString( "%1_%2" ).arg( t ).arg( locale );
    if( trans->load( file, "." ) || trans->load( file, "resources/locales" ) )
        installTranslator( trans );
}

// -----------------------------------------------------------------------------
// Name: Application::Initialize
// Created: SBO 2006-07-05
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
    controllers_ = new Controllers();
    logger_      = new LoggerProxy();
    services_    = new Services( controllers_->controller_, *logger_ );
    simulation_  = new Simulation( controllers_->controller_  );
    workers_     = new Workers();
    network_     = new Network( *services_, *simulation_, *logger_ );
    RcEntityResolver_ABC* rcResolver = new RcEntityResolver( this, *controllers_ );
    staticModel_ = new StaticModel( *controllers_, *rcResolver, *simulation_ );
    model_       = new Model( *controllers_, *staticModel_, *simulation_, *workers_, network_->GetMessageMgr(), *rcResolver );
    profile_     = new Profile( *controllers_, network_->GetMessageMgr(), config_->GetLogin() );
    network_->GetMessageMgr().SetElements( *model_, *profile_ );
    mainWindow_  = new MainWindow( *controllers_, *staticModel_, *model_, *simulation_, *network_, *profile_, *config_, *logger_, expiration_ );
    mainWindow_->show();

    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
    networkTimer_ = new QTimer( this );
    connect( networkTimer_, SIGNAL( timeout()), SLOT( UpdateData() ) );
    networkTimer_->start(10);

    config_->Connect( *network_ );
}

// -----------------------------------------------------------------------------
// Name: Application::UpdateData
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::UpdateData()
{
    try
    {
        workers_->CommitTasks();
        network_->Update();
    } 
    catch( std::exception& e )
    {
        network_->Disconnect();
        QMessageBox::critical( 0, tools::translate( "Application", "SWORD" ), e.what(), QMessageBox::Abort, QMessageBox::Abort );
    }
}
