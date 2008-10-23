// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "BattleCenterLauncherPage.h"
#include "moc_BattleCenterLauncherPage.cpp"
#include "ExerciseList.h"
#include "Multiplayer.h"
#include "ProgressPage.h"
#include "ProcessDialogs.h"
#include "CompositeProcessWrapper.h"
#include "frontend/CreateSession.h"
#include "frontend/JoinExercise.h"
#include "frontend/StartExercise.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
BattleCenterLauncherPage::BattleCenterLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "BattleCenterLauncherPage", "Start multiplayer session" ), previous )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "BattleCenterLauncherPage", "Starting multiplayer session" ), controllers ) )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        exercises_ = new ExerciseList( box, config_ );
        connect( exercises_, SIGNAL( Highlight( const QString& ) ), this, SLOT( SelectExercise( const QString& ) ) );
    }
    AddContent( box );
    AddNextButton( tools::translate( "BattleCenterLauncherPage", "Start" ) , *this, SLOT( StartExercise() ) );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage destructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
BattleCenterLauncherPage::~BattleCenterLauncherPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage::SelectExercise
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void BattleCenterLauncherPage::SelectExercise( const QString& name )
{
    exercise_ = name;
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage::StartExercise
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void BattleCenterLauncherPage::StartExercise()
{
    if( exercise_.isEmpty() || ! dialogs::KillRunningProcesses( this ) )
        return;
    {
        frontend::CreateSession action( config_, exercise_.ascii(), MULTIPLAYER_SESSION );
        action.SetDefaultValues();
        action.SetOption( "session/config/simulation/debug/@networklogger", true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", 20000 );
    }
    boost::shared_ptr< frontend::SpawnCommand > command1( new frontend::StartExercise( config_, exercise_, MULTIPLAYER_SESSION.c_str(), true ) );
    // $$$$ SBO 2008-10-15: launch client optionally...
    boost::shared_ptr< frontend::SpawnCommand > command2( new frontend::JoinExercise( config_, exercise_, MULTIPLAYER_SESSION.c_str(), true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new CompositeProcessWrapper( controllers_.controller_, command1, command2 ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage::Update
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void BattleCenterLauncherPage::Update()
{
    exercises_->Update();
}
