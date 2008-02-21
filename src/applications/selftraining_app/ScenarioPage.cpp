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
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.h>
#include <qlistbox.h>
#include <qtextedit.h>

#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;
using namespace xml;

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

    connect( scenarios_, SIGNAL( selectionChanged() ), SLOT( OnScenarioSelected() ) );
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
// Name: ScenarioPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::Update()
{
    scenarios_->clear();
    scenarios_->insertStringList( frontend::commands::ListExercises( config_ ) );
    scenarios_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::OnStart
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::OnStart()
{

}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::OnScenarioSelected
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::OnScenarioSelected()
{
    teams_->clear();
    if( scenarios_->selectedItem() && !scenarios_->selectedItem()->text().isEmpty() )
    {
        const std::string exercise = scenarios_->selectedItem()->text().ascii();
        xifstream xis( config_.GetExerciseFile( exercise ) );
        std::string orbatFile;
        xis >> start( "exercise" )
                >> start( "orbat" )
                    >> attribute( "file", orbatFile );
        UpdateSides( config_.BuildChildPath( config_.GetExerciseFile( exercise ), orbatFile ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::UpdateSides
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::UpdateSides( const std::string& orbat )
{
    xifstream xis( orbat );
    xis >> start( "orbat" )
            >> start( "sides" )
                >> list( "side", *this, &ScenarioPage::ReadSide );
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::ReadSide
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::ReadSide( xml::xistream& xis )
{
    std::string name, type;
    xis >> attribute( "name", name )
        >> attribute( "type", type );
    teams_->insertItem( name.c_str() );
}
