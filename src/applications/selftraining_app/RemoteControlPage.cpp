// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "RemoteControlPage.h"
#include "moc_RemoteControlPage.cpp"
#include "Config.h"
#include "ExerciseList.h"
#include "Multiplayer.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/ExerciseFilter_ABC.h"
#include "clients_gui/Tools.h"

namespace
{
    struct ExerciseFilter : private boost::noncopyable
                          , public frontend::ExerciseFilter_ABC
    {
        ExerciseFilter( const QLineEdit& host, const QSpinBox& port, bool runningOnly = false )
            : host_( host ), port_( port ), runningOnly_( runningOnly ) {}
        virtual bool Allows( const frontend::Exercise_ABC& exercise ) const
        {
            return exercise.IsHostedBy( QString( "%1:%2" ).arg( host_.text() ).arg( port_.value() ).ascii() )
                && ( !runningOnly_ || runningOnly_ && exercise.IsRunning() );
        }
        const QLineEdit& host_;
        const QSpinBox& port_;
        const bool runningOnly_;
    };
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage constructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
RemoteControlPage::RemoteControlPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers,
                                      const Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack, launcher )
    , controllers_      ( controllers )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , exercise_         ( 0 )
    , runningExercise_  ( 0 )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    {
        Q3GroupBox* gbox = new Q3GroupBox( 1, Qt::Vertical, box );
        hostLabel_ = new QLabel( gbox );
        host_ = new QLineEdit( "127.0.0.1", gbox );
        portLabel_ = new QLabel( gbox );
        port_ = new QSpinBox( gbox );
        port_->setMaxValue( 65535 );
        port_->setValue( config.GetLauncherPort() );
        updateButton_ = new QPushButton( gbox );
        connect( updateButton_, SIGNAL( clicked() ), SLOT( UpdateExerciseList() ) );
    }
    tabs_ = new QTabWidget( box );
    {
        Q3HBox* hbox = new Q3HBox( tabs_ );
        {
            filter_.reset( new ExerciseFilter( *host_, *port_ ) );
            exercises_ = new ExerciseList( hbox, config, fileLoader_, controllers, false, false, false, false );
            exercises_->SetFilter( *filter_ );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
        }
        {
            Q3VBox* vbox = new Q3VBox( hbox );
            {
                gameButton_ = new QPushButton( vbox );
                connect( gameButton_, SIGNAL( clicked() ), SLOT( OnStart() ) );
            }
            {
                replayButton_ = new QPushButton( vbox );
                connect( replayButton_, SIGNAL( clicked() ), SLOT( OnReplay() ) );
            }
        }
        tabs_->addTab( hbox, "" ); // Start
    }
    {
        Q3HBox* hbox = new Q3HBox( tabs_ );
        {
            runningFilter_.reset( new ExerciseFilter( *host_, *port_, true ) );
            runningExercises_ = new ExerciseList( hbox, config, fileLoader_, controllers, false, false, false, false );
            runningExercises_->SetFilter( *runningFilter_ );
            connect( runningExercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectRunningExercise( const frontend::Exercise_ABC& ) ) );
            connect( runningExercises_, SIGNAL( ClearSelection() ), SLOT( ClearRunningSelection() ) );
        }
        {
            Q3VBox* vbox = new Q3VBox( hbox );
            stopButton_ = new QPushButton( vbox );
            connect( stopButton_, SIGNAL( clicked() ), SLOT( OnStop() ) );
        }
        tabs_->addTab( hbox, "" ); // Control
    }
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage destructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
RemoteControlPage::~RemoteControlPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void RemoteControlPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "RemoteControlPage", "Remote control" ) );
    hostLabel_->setText(    tools::translate( "RemoteControlPage", "Host:" ) );
    portLabel_->setText(    tools::translate( "RemoteControlPage", "Port:" ) );
    tabs_->setTabText( 0,   tools::translate( "RemoteControlPage", "Start" ) );
    tabs_->setTabText( 1,   tools::translate( "RemoteControlPage", "Control" ) );
    updateButton_->setText( tools::translate( "RemoteControlPage", "Update" ) );
    gameButton_->setText(   tools::translate( "RemoteControlPage", "Start game" ) );
    replayButton_->setText( tools::translate( "RemoteControlPage", "Start replay" ) );
    stopButton_->setText(   tools::translate( "RemoteControlPage", "Stop running session" ) );
	LauncherClientPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::SelectExercise
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void RemoteControlPage::SelectExercise( const frontend::Exercise_ABC& exercise )
{
    exercise_ = &exercise;
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteControlPage::ClearSelection()
{
    exercise_ = 0;
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::SelectRunningExercise
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void RemoteControlPage::SelectRunningExercise( const frontend::Exercise_ABC& exercise )
{
    runningExercise_ = &exercise;
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::ClearRunningSelection
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void RemoteControlPage::ClearRunningSelection()
{
    runningExercise_ = 0;
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::UpdateExerciseList
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void RemoteControlPage::UpdateExerciseList()
{
    ClearSelection();
    exercises_->Clear();
    Connect( host_->text().ascii(), static_cast< unsigned short >( port_->value() ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnStart
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void RemoteControlPage::OnStart()
{
    if( exercise_ )
        exercise_->Start( MULTIPLAYER_SESSION );
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnReplay
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void RemoteControlPage::OnReplay()
{
    if( exercise_ )
        exercise_->Replay( MULTIPLAYER_SESSION );
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnStop
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteControlPage::OnStop()
{
    if( runningExercise_ && runningExercise_->IsRunning() )
        runningExercise_->Stop( MULTIPLAYER_SESSION );
}
