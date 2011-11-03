// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ReplayPage.h"
#include "moc_ReplayPage.cpp"
#include "ExerciseList.h"
#include "CompositeProcessWrapper.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "SessionList.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/StartReplay.h"
#include "frontend/JoinAnalysis.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ReplayPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::ReplayPage( Q3WidgetStack* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, tools::translate( "ReplayPage", "Replay" ), previous, eButtonBack | eButtonStart, launcher )
    , config_( config )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ReplayPage", "Starting replay session" ) ) )
{
    Q3VBox* mainBox = new Q3VBox( this );
    {
        Q3HBox* hbox = new Q3HBox( mainBox );
        hbox->setMargin( 10 );
        hbox->setSpacing( 10 );
        {
            exercises_ = new ExerciseList( hbox, config, fileLoader_, controllers, false, true, false );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelectExercise( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
        }
        {
            sessions_ = new SessionList( hbox, config, fileLoader_ );
            connect( sessions_, SIGNAL( Select( const QString& ) ), SLOT( OnSelectSession( const QString& ) ) );
        }
    }
    EnableButton( eButtonStart, false );
    AddContent( mainBox );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::~ReplayPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::Update()
{
    exercises_->Clear();
    Connect( "localhost", config_.GetLauncherPort() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::StartExercise
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::StartExercise()
{
    if( !exercise_ || session_.isEmpty() || !profile_.IsValid() || ! dialogs::KillRunningProcesses( this ) )
        return;
    const QString exerciseName = exercise_->GetName().c_str();
    const unsigned int port = exercise_->GetPort();
    ConfigureSession( exerciseName, session_ );
    boost::shared_ptr< frontend::SpawnCommand > replay( new frontend::StartReplay( config_, exerciseName, session_, port, true ) );
    boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinAnalysis( config_, exerciseName, session_, profile_.GetLogin(), port, true ) );
    boost::shared_ptr< CompositeProcessWrapper >  process( new CompositeProcessWrapper( *progressPage_, replay, client ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void ReplayPage::OnStart()
{
    StartExercise();
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnSelectExercise
// Created: RDS 2008-09-02
// -----------------------------------------------------------------------------
void ReplayPage::OnSelectExercise( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile )
{
    if( exercise_ != &exercise )
    {
        session_ = "";
        sessions_->Update( exercise.GetName().c_str() );
    }
    exercise_ = &exercise;
    profile_ = profile;
    EnableButton( eButtonStart, !session_.isEmpty() && profile_.IsValid() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void ReplayPage::ClearSelection()
{
    exercise_ = 0;
    profile_ = frontend::Profile::Invalid;
    session_ = "";
    sessions_->Update( "" );
    EnableButton( eButtonStart, false );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnSelectSession
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void ReplayPage::OnSelectSession( const QString& session )
{
    session_ = session;
    EnableButton( eButtonStart, exercise_ && !session_.isEmpty() && profile_.IsValid() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ConfigureSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void ReplayPage::ConfigureSession( const QString& exercise, const QString& session )
{
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues(); // reset specific parameters
    action.Commit();
}
