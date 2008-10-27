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
#include "ScenarioLauncherPageOptions.h" 
#include "ExerciseList.h" 
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPage", "Scenario" ), previous )
    , lister_( config, "" )
{
    ScenarioLauncherPageOptions* options = new ScenarioLauncherPageOptions( pages, *this, controllers, config );
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        exercises_ = new ExerciseList( box, config, lister_ );
        connect( exercises_, SIGNAL( Highlight( const QString& ) ), options, SLOT( Update( const QString& ) ) );
    }
    AddContent( box );
    AddNextButton( tools::translate( "ScenarioLauncherPage", "Start" ), *options );
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
