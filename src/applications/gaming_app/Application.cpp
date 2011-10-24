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
#include "gaming/AgentServerMsgMgr.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Workers.h"
#include "tools/NullFileLoaderObserver.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;

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
Application::Application( int& argc, char** argv, const QString& expiration )
    : Application_ABC( argc, argv )
    , mainWindow_ ( 0 )
    , expiration_ ( expiration )
{
    const QString locale = ReadLang();
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "actions" );
    AddTranslator( locale, "actions_gui" );
    AddTranslator( locale, "clients_kernel" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "gaming" );
    AddTranslator( locale, "gaming_app" );
    AddTranslator( locale, "reports" );
    AddTranslator( locale, "indicators" );
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
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void Application::Initialize( int /*argc*/, char** /*argv*/ )
{
    observer_.reset( new tools::NullFileLoaderObserver() );
    config_.reset( new Config( argc(), argv(), *observer_ ) );
    controllers_.reset( new Controllers() );
    logger_.reset( new LoggerProxy() );
    services_.reset( new Services( controllers_->controller_, *logger_ ) );
    simulation_.reset( new Simulation( controllers_->controller_ ) );
    workers_.reset( new Workers() );
    network_.reset( new Network( *services_, *simulation_, *logger_, config_->GetNetworkTimeOut() ) );
    rcResolver_.reset( new RcEntityResolver( *controllers_ ) );
    staticModel_.reset( new ::StaticModel( *controllers_, *rcResolver_.get(), *simulation_ ) );
    model_.reset( new Model( *controllers_, *staticModel_, *simulation_, *workers_, network_->GetMessageMgr(), *config_ ) );
    profile_.reset( new Profile( *controllers_, network_->GetMessageMgr(), config_->GetLogin(), config_->IsLoginInCommandLine() ) );
    network_->GetMessageMgr().SetElements( *model_, *profile_ );
    mainWindow_ = new MainWindow( *controllers_, *staticModel_, *model_, *simulation_, *network_, *profile_, *config_, *logger_, *rcResolver_.get(), expiration_ );
    mainWindow_->show();

    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
    networkTimer_ = new QTimer( this );
    connect( networkTimer_, SIGNAL( timeout()), SLOT( UpdateData() ) );
    networkTimer_->start( 10 );

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
        QMessageBox::critical( 0, tools::translate( "Application", "SWORD" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Application::notify
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
bool Application::notify( QObject* pReceiver, QEvent* pEvent )
{
    try
    {
        return QApplication::notify( pReceiver, pEvent );
    }
    catch( std::exception& e )
    {
        DisplayError( e.what() );
    }
    catch( ... )
    {
        DisplayError( tr( "Unknown exception caught" ) );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Application::DisplayError
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Application::DisplayError( const QString& text ) const
{
    static bool active = false;
    if( ! active )
    {
        active = true;
        QMessageBox::critical( activeWindow(),  tools::translate( "Application", "SWORD" ), text, "Ok" );
        active = false;
    }
}
