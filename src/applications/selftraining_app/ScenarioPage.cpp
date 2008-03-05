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
#include "SideList.h"
#include "frontend/commands.h"
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/CreateSession.h"
#include <qlistbox.h>
#include <qtextedit.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

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
    
    QVBox* vBox = new QVBox( box );
    vBox->setSpacing( 10 );
    exercises_ = new QListBox( vBox );
    SideList* sides = new SideList( vBox, config );
    vBox->setStretchFactor( exercises_, 3 );
    vBox->setStretchFactor( sides, 1 );
    box->setStretchFactor( vBox, 5 );
    
    briefing_ = new QTextEdit( box );
    briefing_->setText( "Briefing" );
    briefing_->setReadOnly( true );
    box->setStretchFactor( briefing_, 4 );
    
    AddContent( box );    
    AddNextButton( tr( "Start" ), *this, SLOT( OnStart() ) );

    connect( exercises_, SIGNAL( highlighted( const QString& ) ), sides, SLOT( Update( const QString& ) ) );
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
    exercises_->clear();
    exercises_->insertStringList( frontend::commands::ListExercises( config_ ) );
    exercises_->setSelected( 0, true );
}

namespace
{
    std::string BuildSessionName()
    {
        return bpt::to_iso_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::OnStart
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioPage::OnStart()
{
    if( exercises_->selectedItem() )
    {
        const QString exercise = exercises_->selectedItem()->text();
        const QString session  = BuildSessionName().c_str();
        CreateSession( exercise, session );
        new frontend::StartExercise( this, config_, exercise, session );
        new frontend::JoinExercise ( this, config_, exercise, session );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage::CreateSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void ScenarioPage::CreateSession( const QString& exercise, const QString& session )
{
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues();
}
