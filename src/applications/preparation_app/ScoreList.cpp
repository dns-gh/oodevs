// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreList.h"
#include "moc_ScoreList.cpp"
#include "ScoreEditor.h"
#include "clients_kernel/Controllers.h"
#include "indicators/Primitives.h"
#include "preparation/Score_ABC.h"
#include "preparation/ScoresModel.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ScoreList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::ScoreList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::ParametersLayer& layer, ScoresModel& model, const StaticModel& staticModel, const tools::ExerciseConfig& config )
    : QVBox       ( parent )
    , controllers_( controllers )
    , factory_    ( factory )
    , model_      ( model )
    , scores_     ( new gui::ListDisplayer< ScoreList >( this, *this, factory ) )
    , editor_     ( new ScoreEditor( this, controllers, factory, layer, model, staticModel ) )
    , config_     ( config )
{
    layout()->setAlignment( Qt::AlignRight );
    scores_->AddColumn( tr( "Name" ) );
    {
        QHBox* box = new QHBox( this );
        QButton* editButton = new QPushButton( tr( "Edit..." ), box );
        QButton* deleteButton = new QPushButton( tr( "Delete" ), box );
        connect( editButton, SIGNAL( clicked() ), SLOT( OnEdit() ) );
        connect( deleteButton, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    if( bfs::exists( tools::GeneralConfig::BuildResourceChildFile( "Scores.xml" ) ) )
    {
        QHBox* box = new QHBox( this );
        QButton* defaultIndicators = new QPushButton( tr( "Generate default indicators" ), box );
        connect( defaultIndicators, SIGNAL( clicked() ), SLOT( OnGenerate() ) );
    }
    connect( scores_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnEdit() ) );
    connect( editor_, SIGNAL( Show() ), SIGNAL( Hide() ) );
    connect( editor_, SIGNAL( Hide() ), SIGNAL( Show() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreList destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::~ScoreList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnEdit
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnEdit()
{
    if( Score_ABC* score = FindSelected() )
        editor_->StartEdit( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnDelete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnDelete()
{
    // $$$$ SBO 2009-04-20: prompt confirmation
    if( const Score_ABC* score = FindSelected() )
        emit ScoreDeleted( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnGenerate
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
void ScoreList::OnGenerate()
{
    std::string templateFile = tools::GeneralConfig::BuildResourceChildFile( "Scores.xml" );
    if( bfs::exists( templateFile ) )
        model_.GenerateScoresFromTemplate( templateFile, config_.GetLoader() );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyCreated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyCreated( const Score_ABC& element )
{
    gui::ValuedListItem* item = factory_.CreateItem( scores_ );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyUpdated( const Score_ABC& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyDeleted
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyDeleted( const Score_ABC& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        scores_->removeItem( item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::Display
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::Display( const Score_ABC& score, gui::ValuedListItem* item )
{
    item->SetNamed( score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::FindSelected
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score_ABC* ScoreList::FindSelected() const
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( scores_->selectedItem() ) )
        return item->GetValue< Score_ABC >();
    return 0;
}
