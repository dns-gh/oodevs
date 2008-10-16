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
#include "ExerciseList.h"
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
BattleCenterJoinPage::BattleCenterJoinPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "BattleCenterJoinPage", "Join" ), previous )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "BattleCenterJoinPage", "Joining host" ), controllers ) )
    , config_( config )
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
        port_->setValue( 10001 );
    }
    {
        exercises_ = new ExerciseList( box, config_ );
        connect( exercises_, SIGNAL( Highlight( const QString& ) ), this, SLOT( SelectExercise( const QString& ) ) );
    }
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
void BattleCenterJoinPage::SelectExercise( const QString& name )
{
    exercise_ = name;
}

// -----------------------------------------------------------------------------
// Name: BattleCenterJoinPage::JoinExercise
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void BattleCenterJoinPage::JoinExercise()
{
    if( exercise_.isEmpty() || ! dialogs::KillRunningProcesses( this ) )
        return;
    const std::string session = "multiplayer";
    {
        frontend::CreateSession action( config_, exercise_.ascii(), session );
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( host_->text() ).arg( port_->value() ) );
    }
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::JoinExercise( config_, exercise_, session.c_str(), true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}
