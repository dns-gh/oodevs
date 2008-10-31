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
#include "ProgressPage.h"
#include "CompositeProcessWrapper.h"
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/CreateSession.h"
#include "clients_kernel/Controllers.h" 
#include "clients_gui/Tools.h"

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPage", "Scenario" ), previous )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioLauncherPage", "Starting session" ), controllers ) )
    , lister_( config, "" )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        exercises_ = new ExerciseList( box, config, lister_ );
        connect( exercises_, SIGNAL( Select( const QString&, const QString& ) ), this, SLOT( OnSelect( const QString&, const QString& ) ) );
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

namespace
{
    std::string BuildSessionName()
    {
        return bpt::to_iso_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnStart
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnStart()
{
    if( exercise_.isEmpty() )
        return;
    const QString session = BuildSessionName().c_str();
    CreateSession( exercise_, session );
    boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exercise_, session, true ) );
    boost::shared_ptr< frontend::SpawnCommand > client;
    if( !profile_.isEmpty() )
        client.reset( new frontend::JoinExercise( config_, exercise_, session, profile_, true ) );
    else
        client.reset( new frontend::JoinExercise( config_, exercise_, session, true ) );
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
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues();
    {
        // force the networklogger to be used
        action.SetOption( "session/config/simulation/debug/@networklogger", true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", 20000 );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelect
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelect( const QString& exercise, const QString& profile )
{
    exercise_ = exercise;
    profile_ = profile;
}
