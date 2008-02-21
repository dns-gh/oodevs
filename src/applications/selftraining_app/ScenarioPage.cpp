// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioPage.h"
#include "moc_ScenarioPage.cpp"
#include "frontend/commands.h"
#include <qlistbox.h>
#include <qtextedit.h>

// -----------------------------------------------------------------------------
// Name: ScenarioPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioPage::ScenarioPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config )
    : ContentPage( pages, tr( "Scenario" ), previous )
    , config_( config )
{
    QHBox* box = new QHBox( this );
    box->setSpacing( 50 );
    box->setMargin( 40 );
    {
        QVBox* vBox = new QVBox( box );
        vBox->setSpacing( 10 );
        scenarios_ = new QListBox( vBox );
        teams_ = new QListBox( vBox );
        vBox->setStretchFactor( scenarios_, 3 );
        vBox->setStretchFactor( teams_, 1 );
        box->setStretchFactor( vBox, 5 );
    }
    {
        briefing_ = new QTextEdit( box );
        briefing_->setText( "Briefing" );
        briefing_->setReadOnly( true );
        box->setStretchFactor( briefing_, 4 );
    }
    AddContent( box );    
    AddNextButton( tr( "Start" ), *this, SLOT( OnStart() ) );
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioPage::~ScenarioPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::OnStart
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::OnStart()
{

}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::Update()
{
    scenarios_->clear();
    scenarios_->insertStringList( frontend::commands::ListExercises( config_ ) );
    scenarios_->setSelected( 0, true );
}
