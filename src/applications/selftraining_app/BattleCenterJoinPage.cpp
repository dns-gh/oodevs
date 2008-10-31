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
#include "NetworkExerciseLister.h"
#include "Multiplayer.h"
#include "ProgressPage.h"
#include "ProcessWrapper.h"
#include "ProcessDialogs.h"
#include "frontend/CreateSession.h"
#include "frontend/JoinExercise.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include <qspinbox.h>

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
BattleCenterJoinPage::BattleCenterJoinPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config, NetworkExerciseLister& lister )
    : ContentPage    ( pages, tools::translate( "BattleCenterJoinPage", "Join" ), previous )
    , controllers_   ( controllers )
    , progressPage_  ( new ProgressPage( pages, *this, tools::translate( "BattleCenterJoinPage", "Joining host" ), controllers ) )
    , config_        ( config )
    , exerciseLister_( lister )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QGroupBox* hbox = new QGroupBox( 1, Qt::Vertical, box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "BattleCenterJoinPage", "Host:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        host_ = new QLineEdit( tools::translate( "BattleCenterJoinPage", "localhost" ), hbox );
        label = new QLabel( tools::translate( "BattleCenterJoinPage", "Port:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        port_ = new QSpinBox( hbox );
        port_->setMaxValue( 32000 );
        port_->setValue( config.GetListServerPort() );
        QPushButton* pButton = new QPushButton( tools::translate( "BattleCenterJoinPage", "Update" ), hbox );
        connect( pButton, SIGNAL( clicked() ), this, SLOT( ReloadExerciseList() ) );
    }
    {
        exercises_ = new ExerciseList( box, config_, exerciseLister_ );
        exerciseLister_.AddList( exercises_ );
        connect( exercises_, SIGNAL( Select( const QString&, const QString& ) ), this, SLOT( SelectExercise( const QString&, const QString& ) ) );
    }
    ReloadExerciseList();
    AddContent( box ); 
    AddNextButton( tools::translate( "BattleCenterJoinPage", "Join" ), *this, SLOT( JoinExercise() ) );
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
// Name: BattleCenterJoinPage::Update
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::Update()
{
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::SelectExercise
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::SelectExercise( const QString& name, const QString& profile )
{
    exercise_ = name;
    profile_ = profile;
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::JoinExercise
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::JoinExercise()
{
    if( exercise_.isEmpty() || ! dialogs::KillRunningProcesses( this ) )
        return;
    {
        frontend::CreateSession action( config_, exercise_.ascii(), MULTIPLAYER_SESSION );
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( host_->text() ).arg( exerciseLister_.GetPort( exercise_ ) ) );
    }
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::JoinExercise( config_, exercise_, MULTIPLAYER_SESSION.c_str(), profile_, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::ReloadExerciseList
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::ReloadExerciseList()
{
    exerciseLister_.DownloadExercises( host_->text().ascii(), static_cast< unsigned int >( port_->value() ) );
}
