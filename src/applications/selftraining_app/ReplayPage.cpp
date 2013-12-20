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
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "SessionList.h"

#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/StartReplay.h"
#include "frontend/StartTimeline.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/ProcessWrapper.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"

#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: ReplayPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::ReplayPage( Application& app, QStackedWidget* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonStart, launcher )
    , config_( config )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( app, pages, *this ) )
{
    //Main Window
    setWindowTitle( "ReplayPage" );
    QWidget* mainBox = new QWidget( this );
    QHBoxLayout* mainBoxLayout = new QHBoxLayout( mainBox );
    mainBoxLayout->setMargin( 10 );
    mainBoxLayout->setSpacing( 10 );

    //exercise list
    exercises_ = new ExerciseList( mainBox, config, fileLoader_, controllers, false, true, false );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelectExercise( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    mainBoxLayout->addWidget( exercises_ );

    //session List
    sessions_ = new SessionList( mainBox, config, fileLoader_ );
    connect( sessions_, SIGNAL( Select( const tools::Path& ) ), SLOT( OnSelectSession( const tools::Path& ) ) );

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
// Name: ReplayPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ReplayPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "ReplayPage", "Replay" ) );
    progressPage_->SetTitle( tools::translate( "ReplayPage", "Starting replay session" ) );
    LauncherClientPage::OnLanguageChanged();
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
    if( !exercise_ || session_.IsEmpty() || !profile_.IsValid() || ! dialogs::KillRunningProcesses( parentWidget()->parentWidget() ) )
        return;
    const tools::Path exerciseName = exercise_->GetName();
    const unsigned int port = exercise_->GetPort();
    ConfigureSession( exerciseName, session_ );
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::StartReplay >( config_, exerciseName, session_, port, "" ) );
    process->Add( boost::make_shared< frontend::StartTimeline >( config_, exerciseName, session_ ) );
    process->Add( boost::make_shared< frontend::JoinAnalysis >( config_, exerciseName, session_, profile_.GetLogin() ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
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
        sessions_->Update( exercise.GetName() );
    }
    exercise_ = &exercise;
    profile_ = profile;
    EnableButton( eButtonStart, !session_.IsEmpty() && profile_.IsValid() );
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
void ReplayPage::OnSelectSession( const tools::Path& session )
{
    session_ = session;
    EnableButton( eButtonStart, exercise_ && !session_.IsEmpty() && profile_.IsValid() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ConfigureSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void ReplayPage::ConfigureSession( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetDefaultValues(); // reset specific parameters
    action.Commit();
}
