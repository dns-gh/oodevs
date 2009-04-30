// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ScorePanel.h"
#include "moc_ScorePanel.cpp"
#include "IndicatorPlot.h"
#include "IndicatorPlotFactory.h"
#include "clients_kernel/Controllers.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Score.h"
#include "gaming/ScoreModel.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: ScorePanel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, IndicatorPlotFactory& plotFactory, ScoreModel& model )
    : QDockWindow( mainWindow, "score" )
    , controllers_( controllers )
    , factory_( factory )
    , plotFactory_( plotFactory )
    , model_( model )
    , pendingGraphRequest_( 0 )
{
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tools::translate( "ScorePanel", "Scores" ) );

    {
        scores_ = new gui::ListDisplayer< ScorePanel >( this, *this, factory_ );
        scores_->AddColumn( tools::translate( "Score", "Name" ) );
        scores_->AddColumn( tools::translate( "Score", "Value" ) );
        scores_->AddColumn( tools::translate( "Score", "Tendency" ) );
        scores_->AddColumn( tools::translate( "Score", "Gauge" ) );
        connect( scores_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnContextMenu( QListViewItem*, const QPoint&, int ) ) );
        connect( scores_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnShowGraph() ) );
    }
    setWidget( scores_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::~ScorePanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyCreated
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyCreated( const Score& element )
{
    gui::ValuedListItem* item = factory_.CreateItem( scores_ );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const Score& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyDeleted
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyDeleted( const Score& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        delete item;
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-04-27
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    scores_->clear();
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const IndicatorRequest& request )
{
    if( pendingGraphRequest_ == &request && request.IsDone() )
    {
        IndicatorPlot* plot = plotFactory_.CreatePlot( request );
        plot->show();
        pendingGraphRequest_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::Display( const Score& score, gui::ValuedListItem* item )
{
    item->SetValue( &score );
    score.Display( scores_->GetItemDisplayer( item ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnContextMenu
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void ScorePanel::OnContextMenu( QListViewItem* item, const QPoint& point, int column )
{
    QPopupMenu* menu = new QPopupMenu( scores_ );
    menu->insertItem( tools::translate( "Score", "View graph" ), this, SLOT( OnShowGraph() ) );
    menu->popup( point );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnShowGraph
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void ScorePanel::OnShowGraph()
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( scores_->selectedItem() ) )
        if( Score* score = item->GetValue< Score >() )
            pendingGraphRequest_ = &model_.CreateRequest( *score );
}
