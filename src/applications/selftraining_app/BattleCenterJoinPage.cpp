// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "BattleCenterJoinPage.h"
#include "moc_BattleCenterJoinPage.cpp"
#include "Config.h"
#include "ExerciseList.h"
#include "Multiplayer.h"
#include "ProgressPage.h"
#include "ProcessDialogs.h"
#include "frontend/CreateSession.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/ExerciseFilter_ABC.h"
#include "frontend/JoinExercise.h"
#include "frontend/ProcessWrapper.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include <QtGui/qspinbox.h>

namespace
{
    struct RunningExerciseFilter : private boost::noncopyable
                                 , public frontend::ExerciseFilter_ABC
    {
        RunningExerciseFilter( const QLineEdit& host, const QSpinBox& port )
            : host_( host ), port_( port ) {}
        virtual bool Allows( const frontend::Exercise_ABC& exercise ) const
        {
            return exercise.IsHostedBy( QString( "%1:%2" ).arg( host_.text() ).arg( port_.value() ).ascii() )
                && exercise.IsRunning();
        }
        const QLineEdit& host_;
        const QSpinBox& port_;
    };
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
BattleCenterJoinPage::BattleCenterJoinPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, tools::translate( "BattleCenterJoinPage", "Join" ), previous, eButtonBack | eButtonJoin, launcher )
    , controllers_      ( controllers )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , progressPage_     ( new ProgressPage( pages, *this, tools::translate( "BattleCenterJoinPage", "Joining host" ) ) )
    , exercise_         ( 0 )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        Q3GroupBox* hbox = new Q3GroupBox( 1, Qt::Vertical, box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "BattleCenterJoinPage", "Host:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        host_ = new QLineEdit( tools::translate( "BattleCenterJoinPage", "127.0.0.1" ), hbox );
        label = new QLabel( tools::translate( "BattleCenterJoinPage", "Port:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        port_ = new QSpinBox( hbox );
        port_->setMaxValue( 65535 );
        port_->setValue( config.GetLauncherPort() );
        QPushButton* pButton = new QPushButton( tools::translate( "BattleCenterJoinPage", "Update" ), hbox );
        connect( pButton, SIGNAL( clicked() ), SLOT( UpdateExerciseList() ) );
    }
    {
        filter_.reset( new RunningExerciseFilter( *host_, *port_ ) );
        exercises_ = new ExerciseList( box, config_, fileLoader_, controllers, true, true, true, false );
        exercises_->SetFilter( *filter_ );
        connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( SelectExercise( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
        connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    }
    EnableButton( eButtonJoin, false );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
BattleCenterJoinPage::~BattleCenterJoinPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::SelectExercise
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::SelectExercise( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile )
{
    exercise_ = &exercise;
    profile_ = profile;
    EnableButton( eButtonJoin, profile_.IsValid() );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::ClearSelection()
{
    exercise_ = 0;
    profile_ = frontend::Profile::Invalid;
    EnableButton( eButtonJoin, false );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::OnJoin
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::OnJoin()
{
    if( !exercise_ || !profile_.IsValid() || ! dialogs::KillRunningProcesses( this ) )
        return;
    {
        frontend::CreateSession action( config_, exercise_->GetName(), MULTIPLAYER_SESSION );
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( host_->text() ).arg( exercise_->GetPort() ) );
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::JoinExercise( config_, exercise_->GetName().c_str(), MULTIPLAYER_SESSION.c_str(), profile_.GetLogin(), true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::UpdateExerciseList
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::UpdateExerciseList()
{
    exercises_->Clear();
    Connect( host_->text().ascii(), static_cast< unsigned short >( port_->value() ) );
}
