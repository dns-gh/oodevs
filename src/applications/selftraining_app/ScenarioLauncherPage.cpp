// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioLauncherPage.h"
#include "moc_ScenarioLauncherPage.cpp"
#include "ScenarioLauncherPageOptions.h" 
#include "ExerciseList.h" 
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, SessionRunningPage& running, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPage", "Scenario" ), previous )
    , config_( config )
    , sessionStatus_( sessionStatus )
{
    options_ = new ScenarioLauncherPageOptions( pages, *this, controllers, running, config );
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    exercises_ = new ExerciseList( box, config );
    QHBox* hbox = new QHBox( box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    hbox->layout()->setAlignment( Qt::AlignRight );
    connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) );
    connect( exercises_, SIGNAL( Highlight( const QString& ) ), options_, SLOT( Update( const QString& ) ) );
    AddContent( box );
    AddNextButton( tools::translate( "ScenarioLauncherPage", "Start" ) , *options_ );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::~ScenarioLauncherPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::Update()
{
    exercises_->Update();
}
