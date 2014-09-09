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
#include "GamingConfig.h"
#include "LoggerProxy.h"
#include "KnowledgeConverter.h"
#include "RcEntityResolver.h"
#include "gaming/Network.h"
#include "gaming/StaticModel.h"
#include "gaming/Model.h"
#include "gaming/Services.h"
#include "gaming/Simulation.h"
#include "gaming/SimulationController.h"
#include "gaming/Profile.h"
#include "gaming/ProfileFilter.h"
#include "gaming/CommandHandler.h"
#include "gaming/AgentServerMsgMgr.h"
#include "clients_gui/VerticalHeaderTableView.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Workers.h"
#include "ENT/ENT_Tr.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Application::Application
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Application::Application( gui::ApplicationMonitor& monitor, int& argc, char** argv )
    : Application_ABC( monitor )
{
    // Application_ABC initialization
    Initialize();

    // Data
    config_.reset( new GamingConfig( argc, argv ) );
    LoadCommandLineLanguage( config_->GetLanguages(), config_->GetCommandLineLanguage() );
    controllers_.reset( new Controllers() );
    logger_.reset( new LoggerProxy() );
    services_.reset( new ::Services( controllers_->controller_, *logger_ ) );
    simulation_.reset( new Simulation( controllers_->controller_ ) );
    workers_.reset( new Workers() );
    network_.reset( new Network( *services_, *simulation_, *logger_, config_->GetNetworkTimeOut() ) );
    simulationController_.reset( new SimulationController( *simulation_, *controllers_, network_->GetMessageMgr() ) );
    knowledgeConverter_.reset( new KnowledgeConverter( *controllers_ ) );
    rcResolver_.reset( new RcEntityResolver( *controllers_, *knowledgeConverter_ ) );
    staticModel_.reset( new ::StaticModel( *controllers_, *rcResolver_, *simulation_ ) );
    const std::string& login = config_->GetLogin();
    const std::string& pwd = config_->GetPassword();
    profile_.reset( new Profile( *controllers_, network_->GetMessageMgr(), login, config_->IsLoginInCommandLine() ) );
    filter_.reset( new ProfileFilter( *controllers_, *profile_ ) );
    if( !pwd.empty() )
        profile_->SetCredentials( login, pwd );
    model_.reset( new Model( *controllers_, *staticModel_, *simulation_, *workers_, network_->GetMessageMgr(), *config_, *profile_, *filter_ ) );
    network_->GetMessageMgr().SetElements( *model_, *profile_ );

    // Network
    networkTimer_.reset( new QTimer( this ) );
    connect( networkTimer_.get(), SIGNAL( timeout()), SLOT( UpdateData() ) );

    //Qt
    QStringList libList = qApp->libraryPaths();
    libList.append( "." );
    qApp->setLibraryPaths( libList );

    // GUI
    mainWindow_.reset( new MainWindow( *controllers_, *staticModel_, *model_, *simulation_, *simulationController_,
        *network_, *filter_, *config_, *logger_, *knowledgeConverter_, *workers_, GetExpiration() ) );
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
    AddTranslator( "actions_gui_parameters" );
    AddTranslator( "clients_kernel" );
    AddTranslator( "clients_gui" );
    AddTranslator( "clients_gui_app6" );
    AddTranslator( "gaming" );
    AddTranslator( "gaming_app" );
    AddTranslator( "indicators" );
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
    mainWindow_->show();
    networkTimer_->start( 10 );
    config_->Connect( *network_ );
    return qApp->exec();
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
    catch( const std::exception& e )
    {
        network_->Disconnect();
        QMessageBox::critical( 0, tools::translate( "Application", "SWORD" ), tools::GetExceptionMsg( e ).c_str() );
    }
}
