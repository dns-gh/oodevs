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
#include "IndicatorExportDialog.h"
#include "IndicatorPlot.h"
#include "IndicatorPlotFactory.h"
#include "IndicatorReportDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedDragObject.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Score.h"
#include "gaming/ScoreModel.h"
#include "gaming/Tools.h"

namespace
{
    class MyList : public gui::ListDisplayer< ScorePanel >
    {
    public:
        MyList( ScorePanel* parent, gui::ItemFactory_ABC& factory, ScoreModel& model )
            : ListDisplayer< ScorePanel >( parent, *parent, factory )
            , model_( model )
        {}
        virtual Q3DragObject* dragObject()
        {
            if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( selectedItem() ) )
                if( Score* score = item->GetValue< Score >() )
                {
                    const IndicatorRequest& request = model_.CreateRequest( *score );
                    return new gui::ValuedDragObject( &request, this );
                }
            return 0;
        }

    private:
        MyList& operator=( const MyList& );
        ScoreModel& model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScorePanel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::LinkInterpreter_ABC& interpreter, IndicatorPlotFactory& plotFactory, IndicatorExportDialog& exportDialog, ScoreModel& model, const tools::ExerciseConfig& config )
    : QDockWidget( "score", mainWindow )
    , controllers_( controllers )
    , factory_( factory )
    , plotFactory_( plotFactory )
    , model_( model )
    , exportDialog_( exportDialog )
    , reportDialog_( new IndicatorReportDialog( this, model_, config, interpreter ) )
{
    setObjectName( "scorePanel" );
    setCaption( tools::translate( "ScorePanel", "Scores" ) );

    {
        scores_ = new MyList( this, factory_, model_ );
        scores_->AddColumn( tools::translate( "Score", "Name" ) );
        scores_->AddColumn( tools::translate( "Score", "Value" ) );
        scores_->AddColumn( tools::translate( "Score", "Tendency" ) );
        scores_->AddColumn( tools::translate( "Score", "Gauge" ) );
        scores_->setColumnWidthMode( 2, Q3ListView::Manual );
        scores_->setColumnWidthMode( 3, Q3ListView::Manual );
        scores_->setColumnAlignment( 2, Qt::AlignCenter );
        scores_->setColumnAlignment( 3, Qt::AlignCenter );
        connect( scores_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnContextMenu( Q3ListViewItem*, const QPoint&, int ) ) );
        connect( scores_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnShowGraph() ) );
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
    if( request.IsDone() )
    {
        T_PendingRequests::iterator it = std::find( graphRequests_.begin(), graphRequests_.end(), &request );
        if( it != graphRequests_.end() )
        {
            IndicatorPlot* plot = plotFactory_.CreatePlot( request );
            plot->show();
            std::swap( *it, graphRequests_.back() );
            graphRequests_.pop_back();
        }
        else
        {
            T_PendingRequests::iterator it = std::find( exportRequests_.begin(), exportRequests_.end(), &request );
            if( it != exportRequests_.end() )
            {
                exportDialog_.Add( request );
                exportDialog_.Export();
                std::swap( *it, exportRequests_.back() );
                exportRequests_.pop_back();
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::Display( const Score& score, gui::ValuedListItem* item )
{
    item->setDragEnabled( true );
    item->SetValue( &score );
    score.Display( scores_->GetItemDisplayer( item ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnContextMenu
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void ScorePanel::OnContextMenu( Q3ListViewItem* item, const QPoint& point, int /*column*/ )
{
    Q3PopupMenu* menu = new Q3PopupMenu( scores_ );
    if( item )
    {
        menu->insertItem( tools::translate( "ScorePanel", "View graph" ), this, SLOT( OnShowGraph() ) );
        menu->insertItem( tools::translate( "Indicators", "Export score data..." ), this, SLOT( OnExportData() ) );
        menu->insertSeparator();
    }
    menu->insertItem( tools::translate( "Indicators", "Create report..." ), reportDialog_, SLOT( show() ) );
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
            graphRequests_.push_back( &model_.CreateRequest( *score ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnExportData
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScorePanel::OnExportData()
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( scores_->selectedItem() ) )
        if( Score* score = item->GetValue< Score >() )
            exportRequests_.push_back( &model_.CreateRequest( *score ) );
}
