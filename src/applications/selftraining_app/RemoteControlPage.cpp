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
#include <qspinbox.h>
#include <qtabwidget.h>

namespace
{
    struct ExerciseFilter : public boost::noncopyable
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
RemoteControlPage::RemoteControlPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, tools::translate( "RemoteControlPage", "Remote control" ), previous, eButtonBack, launcher )
    , controllers_      ( controllers )
    , config_           ( config )
    , exercise_         ( 0 )
    , runningExercise_  ( 0 )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QGroupBox* gbox = new QGroupBox( 1, Qt::Vertical, box );
        gbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "RemoteControlPage", "Host:" ), gbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        host_ = new QLineEdit( tools::translate( "RemoteControlPage", "127.0.0.1" ), gbox );
        label = new QLabel( tools::translate( "RemoteControlPage", "Port:" ), gbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        port_ = new QSpinBox( gbox );
        port_->setMaxValue( 65535 );
        port_->setValue( config.GetLauncherPort() );
        QPushButton* pButton = new QPushButton( tools::translate( "RemoteControlPage", "Update" ), gbox );
        connect( pButton, SIGNAL( clicked() ), SLOT( UpdateExerciseList() ) );
    }
    QTabWidget* tabWidget = new QTabWidget( box );
    {
        QHBox* hbox = new QHBox( tabWidget );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        {
            filter_.reset( new ExerciseFilter( *host_, *port_ ) );
            exercises_ = new ExerciseList( hbox, config, controllers, "", false, false, false, false );
            exercises_->SetFilter( *filter_ );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
        }
        {
            QVBox* vbox = new QVBox( hbox );
            vbox->setBackgroundOrigin( QWidget::WindowOrigin );
            {
                QPushButton* button = new QPushButton( tools::translate( "RemoteControlPage", "Start new session" ), vbox );
                connect( button, SIGNAL( clicked() ), SLOT( OnStartSession() ) );
            }
        }
        tabWidget->addTab( hbox, tools::translate( "RemoteControlPage", "Start" ) );
    }
    {
        QHBox* hbox = new QHBox( tabWidget );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        {
            runningFilter_.reset( new ExerciseFilter( *host_, *port_, true ) );
            runningExercises_ = new ExerciseList( hbox, config, controllers, "", false, false, false, false );
            runningExercises_->SetFilter( *runningFilter_ );
            connect( runningExercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const Profile& ) ), SLOT( SelectRunningExercise( const frontend::Exercise_ABC& ) ) );
            connect( runningExercises_, SIGNAL( ClearSelection() ), SLOT( ClearRunningSelection() ) );
        }
        {
            QVBox* vbox = new QVBox( hbox );
            vbox->setBackgroundOrigin( QWidget::WindowOrigin );
            {
                QPushButton* button = new QPushButton( tools::translate( "RemoteControlPage", "Stop running session" ), vbox );
                connect( button, SIGNAL( clicked() ), SLOT( OnStopSession() ) );
            }
        }
        tabWidget->addTab( hbox, tools::translate( "RemoteControlPage", "Control" ) );
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
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnStartSession
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void RemoteControlPage::OnStartSession()
{
    if( exercise_ )
        exercise_->StartSession( MULTIPLAYER_SESSION );
}

// -----------------------------------------------------------------------------
// Name: RemoteControlPage::OnStopSession
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteControlPage::OnStopSession()
{
    if( runningExercise_ && runningExercise_->IsRunning() )
        runningExercise_->StopSession( MULTIPLAYER_SESSION );
}
