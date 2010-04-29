// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioLauncherPage.h"
#include "moc_ScenarioLauncherPage.cpp"
#include "ExerciseList.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "CompositeProcessWrapper.h"
#include "MessageDialog.h"
#include "frontend/CheckpointConfigPanel.h"
#include "frontend/CreateSession.h"
#include "frontend/CrossbowPluginConfigPanel.h"
#include "frontend/DisPluginConfigPanel.h"
#include "frontend/HlaPluginConfigPanel.h"
#include "frontend/JoinExercise.h"
#include "frontend/StartExercise.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include <boost/foreach.hpp>
#include <qtabwidget.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config, const QString& title /*= ""*/ )
    : ContentPage( pages, title.isEmpty() ? tools::translate( "ScenarioLauncherPage", "Scenario" ) : title, previous )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioLauncherPage", "Starting %1" ).arg( title ), controllers ) )
    , lister_( config, "" )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        QTabWidget* tabs = new QTabWidget( box );
        tabs->setMargin( 5 );
        {
            exercises_ = new ExerciseList( tabs, config_, lister_ );
            tabs->addTab( exercises_, tr( "General" ) );
            connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), this, SLOT( OnSelect( const QString&, const Profile& ) ) );
        }
        {
            frontend::CheckpointConfigPanel* panel = AddPlugin< frontend::CheckpointConfigPanel >( tabs, tools::translate( "ScenarioLauncherPage", "Checkpoints" ) );
            connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), panel, SLOT( Select( const QString& ) ) );
            connect( panel, SIGNAL( CheckpointSelected( const QString&, const QString& ) ), SLOT( OnSelectCheckpoint( const QString&, const QString& ) ) );
        }
        AddPlugin< frontend::DisPluginConfigPanel >( tabs, tools::translate( "ScenarioLauncherPage", "DIS" ) );
        AddPlugin< frontend::HlaPluginConfigPanel >( tabs, tools::translate( "ScenarioLauncherPage", "HLA" ) );
        AddPlugin< frontend::CrossbowPluginConfigPanel >( tabs, tools::translate( "ScenarioLauncherPage", "Crossbow" ) );
    }
    AddContent( box );
    AddNextButton( tools::translate( "ScenarioLauncherPage", "Start" ), *this, SLOT( OnStart() ) );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::~ScenarioLauncherPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::Update()
{
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::BuildSessionName
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
std::string ScenarioLauncherPage::BuildSessionName() const
{
    return bpt::to_iso_string( bpt::second_clock::local_time() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnStart
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnStart()
{
    if( exercise_.isEmpty() || !profile_.IsValid() || ! dialogs::KillRunningProcesses( this ) )
        return;
    const QString session = session_.isEmpty() ? BuildSessionName().c_str() : session_;
    CreateSession( exercise_, session );
    boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exercise_, session, checkpoint_, true ) );
    boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinExercise( config_, exercise_, session, profile_.GetLogin(), true ) );
    boost::shared_ptr< frontend::Process_ABC > process( new CompositeProcessWrapper( controllers_.controller_, simulation, client ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::CreateSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::CreateSession( const QString& exercise, const QString& session )
{
    {
        frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
        action.SetDefaultValues();
        // force the networklogger to be used
        action.SetOption( "session/config/simulation/debug/@networklogger", true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", 20000 );
    }
    {
        BOOST_FOREACH( const T_Plugins::value_type& plugin, plugins_ )
            plugin->Commit( exercise.ascii(), session.ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelect
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelect( const QString& exercise, const Profile& profile )
{
    exercise_ = exercise;
    profile_ = profile;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelectCheckpoint
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelectCheckpoint( const QString& session, const QString& checkpoint )
{
    session_ = session;
    checkpoint_ = checkpoint;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::AddPlugin
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
template< typename T >
T* ScenarioLauncherPage::AddPlugin( QTabWidget* tabs, const QString& name )
{
    T* plugin = new T( tabs, config_ );
    tabs->addTab( plugin, name );
    plugins_.push_back( plugin );
    return plugin;
}
