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
#include "DebugConfigPanel.h"
#include "ExerciseContainer.h"
#include "ExerciseList.h"
#include "ProgressPage.h"
#include "ProcessDialogs.h"
#include "frontend/CreateSession.h"
#include "frontend/DebugConfig.h"
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
ScenarioJoinPage::ScenarioJoinPage( Application& app, QStackedWidget* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, const tools::Loader_ABC& fileLoader, ExerciseContainer& exercises, const frontend::DebugConfig* debug )
    : ContentPage( pages, previous, eButtonBack | eButtonJoin )
    , controllers_      ( controllers )
    , exerciseContainer_( exercises )
    , config_           ( config )
    , debug_            ( debug )
    , fileLoader_       ( fileLoader )
    , progressPage_     ( new ProgressPage( app, pages, *this ) )
    , exercise_         ( 0 )
{
    setWindowTitle( "ScenarioJoinPage" );

    tabs_ = new QTabWidget();
    hostLabel_ = new QLabel();
    host_ = new QLineEdit( "127.0.0.1");
    portLabel_ = new QLabel();
    port_ = new QSpinBox();
    port_->setMaximum( 65535 );
    port_->setValue( 10001 );
    timelineLabel_ = new QLabel();
    timeline_ = new QSpinBox();
    timeline_->setMaximum( 65535 );
    timeline_->setValue( 10002 );

    auto gen = new QWidget( this );
    auto genlayout = new QVBoxLayout( gen );
    genlayout->setMargin( 10 );
    genlayout->setSpacing( 10 );
    {
        auto w = new QGroupBox( this );
        auto layout = new QHBoxLayout( w );
        layout->setSizeConstraint( QLayout::SetMinAndMaxSize );
        layout->addWidget( hostLabel_ );
        layout->addWidget( host_ );
        layout->addWidget( portLabel_ );
        layout->addWidget( port_ );
        layout->addWidget( timelineLabel_ );
        layout->addWidget( timeline_ );
        genlayout->addWidget( w );
    }
    {
        exercises_ = new ExerciseList( app, gen, config_, fileLoader_, controllers, true, false, true, false );
        connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC& ) ) );
        connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
        genlayout->addWidget( exercises_ );
    }
    tabs_->addTab( gen, "" );

    EnableButton( eButtonJoin, false );
    auto w = new QWidget( this );
    auto layout = new QHBoxLayout( w );
    layout->addWidget( tabs_ );
    AddContent( w );
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
    tabs_->setTabText( 0, tools::translate( "ScenarioJoinPage", "General" ) );
    tabs_->setTabText( 1, tools::translate( "ScenarioJoinPage", "Debug" ) );
    progressPage_->SetTitle( tools::translate( "ScenarioJoinPage", "Joining host" ) );
    hostLabel_->setText(     tools::translate( "ScenarioJoinPage", "Host" ) );
    portLabel_->setText(     tools::translate( "ScenarioJoinPage", "Port" ) );
    timelineLabel_->setText( tools::translate( "ScenarioJoinPage", "Timeline" ) );

    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: RBA 2012-10-17
// -----------------------------------------------------------------------------
void ScenarioJoinPage::Update()
{
    exercises_->Clear();
    exerciseContainer_.Refresh();
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
    QString features;
    tools::Path sessionDir, timelineLog, cefLog;
    bool mapnik = false;
    {
        frontend::CreateSession action( config_, exercise_->GetName(), "remote" );
        sessionDir = action.GetPath().Parent();
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( host_->text() ).arg( port_->text() ) );
        action.SetOption( "session/config/timeline/@url", QString( "%1:%2" ).arg( host_->text() ).arg( timeline_->text() ) );
        if( debug_ )
        {
            if( debug_->timeline.legacyTimeline )
                action.SetOption( "session/config/timeline/@enabled", "true" );
            timelineLog = debug_->timeline.clientLogPath;
            cefLog = debug_->timeline.cefLog;
            features = debug_->GetDevFeatures();
            mapnik = debug_->gaming.hasMapnik;
        }
        action.Commit();
    }

    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::JoinExercise >( config_,
        exercise_->GetName(), "remote", static_cast< const QString* >( 0 ),
        sessionDir, features, timelineLog, cefLog, mapnik ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}
