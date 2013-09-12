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
#include "clients_kernel/Tools.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: ScenarioJoinPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ScenarioJoinPage::ScenarioJoinPage( Application& app, QStackedWidget* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonJoin, launcher )
    , controllers_      ( controllers )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , progressPage_     ( new ProgressPage( app, pages, *this ) )
    , exercise_         ( 0 )
{
    setWindowTitle( "ScenarioJoinPage" );

    //group box
    hostLabel_ = new QLabel();
    host_ = new QLineEdit( "127.0.0.1");
    portLabel_ = new QLabel();
    port_ = new QSpinBox();
    port_->setMaximum( 65535 );
    port_->setValue( 10001 );

    //sub layout
    QGroupBox* hbox = new QGroupBox();
    QHBoxLayout* hboxLayout = new QHBoxLayout( hbox );
    hboxLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    hboxLayout->addWidget( hostLabel_ );
    hboxLayout->addWidget( host_ );
    hboxLayout->addWidget( portLabel_ );
    hboxLayout->addWidget( port_ );

    QWidget* box = new QWidget( this );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->setMargin( 10 );
    boxLayout->setSpacing( 10 );
    boxLayout->addWidget( hbox );
    {
        exercises_ = new ExerciseList( box, config_, fileLoader_, controllers, true, false, true, false );
        connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC& ) ) );
        connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
        boxLayout->addWidget( exercises_ );
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
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::JoinExercise >( config_, exercise_->GetName(), "remote" ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}
