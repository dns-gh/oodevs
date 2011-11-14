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
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ScoreList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::ScoreList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::ParametersLayer& layer,
                      ScoresModel& model, const StaticModel& staticModel, const tools::ExerciseConfig& config,
                      const kernel::GlTools_ABC& tools )
    : Q3VBox       ( parent )
    , controllers_( controllers )
    , factory_    ( factory )
    , model_      ( model )
    , scores_     ( new gui::ListDisplayer< ScoreList >( this, *this, factory ) )
    , editor_     ( new ScoreEditor( this, controllers, factory, layer, model, staticModel, tools ) )
    , config_     ( config )
{
    layout()->setAlignment( Qt::AlignRight );
    scores_->AddColumn( tr( "Name" ) );
    {
        Q3HBox* box = new Q3HBox( this );
        QPushButton* editButton = new QPushButton( tr( "Edit..." ), box );
        QPushButton* deleteButton = new QPushButton( tr( "Delete" ), box );
        connect( editButton, SIGNAL( clicked() ), SLOT( OnEdit() ) );
        connect( deleteButton, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    {
        generatorBox_ = new Q3HBox( this );
        QPushButton* defaultIndicators = new QPushButton( tr( "Generate default indicators" ), generatorBox_ );
        connect( defaultIndicators, SIGNAL( clicked() ), SLOT( OnGenerate() ) );
        generatorBox_->hide();
    }
    connect( scores_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnEdit() ) );
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
    if( bfs::exists( config_.GetOptionalPhysicalChildFile( "scores" ) ) )
        model_.GenerateScoresFromTemplate( config_.GetLoader() );
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

// -----------------------------------------------------------------------------
// Name: ScoreList::Load
// Created: ABR 2011-05-27
// -----------------------------------------------------------------------------
void ScoreList::Load()
{
    if( bfs::exists( config_.GetOptionalPhysicalChildFile( "scores" ) ) )
        generatorBox_->show();
}

// -----------------------------------------------------------------------------
// Name: ScoreList::Draw
// Created: FPO 2011-07-19
// -----------------------------------------------------------------------------
void ScoreList::Draw( kernel::Viewport_ABC& viewport )
{
    editor_->Draw( viewport );
}
