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
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Score.h"
#include "gaming/ScoreModel.h"

#pragma warning( disable : 4355 )

Q_DECLARE_METATYPE( Score* )

namespace
{
    class MyList : public QTreeWidget
    {
    public:
        MyList( ScorePanel* parent, ScoreModel& model )
            : QTreeWidget( parent )
            , parent_( parent )
            , model_( model )
        {
            setColumnCount( 4 );
            QStringList headers;
            headers << tools::translate( "Score", "Name" ) << tools::translate( "Score", "Value" )
                    << tools::translate( "Score", "Tendency" ) << tools::translate( "Score", "Gauge" );
            setHeaderLabels( headers );
            setAllColumnsShowFocus( true );
            setDragEnabled( true );
            setRootIsDecorated( false );
        }
        virtual void startDrag( Qt::DropActions /*supportedActions*/ )
        {
            const QModelIndex index = selectionModel()->currentIndex();
            if( index.isValid() )
            {
                if( Score* score = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< Score* >() )
                {
                    IndicatorRequest& request = model_.CreateRequest( *score );
                    dnd::CreateDragObject( &request, this );
                }
            }
        }

        void contextMenuEvent( QContextMenuEvent* event )
        {
            if( !event )
                return;
            kernel::ContextMenu* menu = new kernel::ContextMenu( this );
            if( indexAt( event->pos() ).isValid() )
            {
                menu->insertItem( tools::translate( "ScorePanel", "View graph" ), parent_, SLOT( OnShowGraph() ) );
                menu->insertItem( tools::translate( "Indicators", "Export score data..." ), parent_, SLOT( OnExportData() ) );
                menu->insertSeparator();
            }
            menu->popup( event->globalPos() );
        }

    private:
        ScorePanel* parent_;
        ScoreModel& model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScorePanel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor, IndicatorPlotFactory& plotFactory, IndicatorExportDialog& exportDialog, ScoreModel& model )
    : gui::RichDockWidget( controllers, mainWindow, "score" )
    , extractor_( extractor )
    , plotFactory_( plotFactory )
    , model_( model )
    , exportDialog_( exportDialog )
{
    setObjectName( "scores" );
    setCaption( tools::translate( "ScorePanel", "Scores" ) );
    scores_ = new MyList( this, model_ );
    connect( scores_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnShowGraph() ) );
    setWidget( scores_ );
    controllers_.Update( *this );
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
    QTreeWidgetItem* item = new QTreeWidgetItem( scores_ );
    item->setData( 0, Qt::UserRole, QVariant::fromValue( const_cast< Score* >( &element ) ) );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const Score& element )
{
    for( int i = 0; i < scores_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = scores_->topLevelItem( i );
        if( item && item->data( 0, Qt::UserRole ).value< Score* >() == &element )
        {
            Display( element, item );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyDeleted
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyDeleted( const Score& element )
{
    for( int i = 0; i < scores_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = scores_->topLevelItem( i );
        if( item && item->data( 0, Qt::UserRole ).value< Score* >() == &element )
        {
            delete scores_->takeTopLevelItem( i );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-04-27
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    while( scores_->topLevelItemCount() > 0 )
        delete scores_->takeTopLevelItem( 0 );
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
void ScorePanel::Display( const Score& score, QTreeWidgetItem* item )
{
    item->setText( 0, score.GetName() );
    score.Display( item, extractor_, 1, 2, 3 );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnShowGraph
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void ScorePanel::OnShowGraph()
{
    const QModelIndex index = scores_->selectionModel()->currentIndex();
    if( index.isValid() )
        if( Score* score = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< Score* >() )
            graphRequests_.push_back( &model_.CreateRequest( *score ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::OnExportData
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void ScorePanel::OnExportData()
{
    const QModelIndex index = scores_->selectionModel()->currentIndex();
    if( index.isValid() )
        if( Score* score = index.model()->data( index.model()->index( index.row(), 0, index.parent() ), Qt::UserRole ).value< Score* >() )
            exportRequests_.push_back( &model_.CreateRequest( *score ) );
}
