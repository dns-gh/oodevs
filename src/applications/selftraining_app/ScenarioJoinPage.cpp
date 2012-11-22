// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioJoinPage.h"
#include "moc_ScenarioJoinPage.cpp"
#include "Config.h"
#include "ExerciseList.h"
#include "ProgressPage.h"
#include "ProcessDialogs.h"
#include "frontend/CreateSession.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/JoinExercise.h"
#include "frontend/ProcessWrapper.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ScenarioJoinPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ScenarioJoinPage::ScenarioJoinPage( Application& app, Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonJoin, launcher )
    , controllers_      ( controllers )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , progressPage_     ( new ProgressPage( app, pages, *this ) )
    , exercise_         ( 0 )
{
    setName( "ScenarioJoinPage" );
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    {
        QGroupBox* hbox = new QGroupBox( box );
        hostLabel_ = new QLabel();
        host_ = new QLineEdit( "127.0.0.1");
        portLabel_ = new QLabel();
        port_ = new QSpinBox();
        port_->setMaxValue( 65535 );
        port_->setValue( 10001 );

        QHBoxLayout* hboxLayout = new QHBoxLayout( hbox );
        hboxLayout->addWidget( hostLabel_ );
        hboxLayout->addWidget( host_ );
        hboxLayout->addWidget( portLabel_ );
        hboxLayout->addWidget( port_ );
    }
    {
        exercises_ = new ExerciseList( box, config_, fileLoader_, controllers, true, false, true, false );
        connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC& ) ) );
        connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    }
    EnableButton( eButtonJoin, false );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: ScenarioJoinPage destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ScenarioJoinPage::~ScenarioJoinPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioJoinPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ScenarioJoinPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "ScenarioJoinPage", "Join" ) );
    progressPage_->SetTitle( tools::translate( "ScenarioJoinPage", "Joining host" ) );
    hostLabel_->setText(     tools::translate( "ScenarioJoinPage", "Host:" ) );
    portLabel_->setText(     tools::translate( "ScenarioJoinPage", "Port:" ) );
    LauncherClientPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: RBA 2012-10-17
// -----------------------------------------------------------------------------
void ScenarioJoinPage::Update()
{
    exercises_->Clear();
    Connect( "localhost", config_.GetLauncherPort() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::SelectExercise
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ScenarioJoinPage::SelectExercise( const frontend::Exercise_ABC& exercise )
{
    exercise_ = &exercise;
    EnableButton( eButtonJoin, true );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void ScenarioJoinPage::ClearSelection()
{
    exercise_ = 0;
    EnableButton( eButtonJoin, false );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnJoin
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ScenarioJoinPage::OnJoin()
{
    if( !exercise_ )
        return;
    {
        frontend::CreateSession action( config_, exercise_->GetName(), "remote" );
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( host_->text() ).arg( port_->text() ) );
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::JoinExercise( config_, exercise_->GetName().c_str(), "remote", true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}
