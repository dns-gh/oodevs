// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineView.h"
#include "TimelineMarker.h"
#include "TimelineActionItem.h"
#include "TimelineRuler.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionTasker.h"
#include "gaming/Simulation.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "icons.h"
#include <qpainter.h>

using namespace kernel;
using namespace actions;

const unsigned int TimelineView::rowHeight_ = 25;

// -----------------------------------------------------------------------------
// Name: TimelineView constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineView::TimelineView( QWidget* parent, QCanvas* canvas, Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, TimelineRuler& ruler )
    : QCanvasView  ( canvas, parent, "TimelineView" )
    , controllers_ ( controllers )
    , model_       ( model )
    , ruler_       ( ruler )
    , marker_      ( new TimelineMarker( canvas, scheduler, controllers_, ruler_ ) )
    , selectedItem_( 0 )
    , filter_      ( 0 )
{
    // initialize some elements needed in action tooltips
    QMimeSourceFactory::defaultFactory()->setPixmap( "mission", MAKE_PIXMAP( mission ) );

    viewport()->setMouseTracking( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineView::~TimelineView()
{
    QMimeSourceFactory::defaultFactory()->setData( "mission", 0 );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyCreated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyCreated( const Action_ABC& action )
{
    const kernel::Entity_ABC* entity = 0;
    if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
        entity = tasker->GetTasker();
    T_Entities::iterator it = std::find( entities_.begin(), entities_.end(), entity );
    if( it == entities_.end() )
    {
        entities_.push_back( entity );
        canvas()->resize( canvas()->width(), canvas()->height() + rowHeight_ );
    }
    actions_[ entity ][ &action ] = new TimelineActionItem( canvas(), ruler_, controllers_, model_, action );

    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const Action_ABC& action )
{
    const kernel::Entity_ABC* entity = 0;
    if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
        entity = tasker->GetTasker();

    T_EntityActions::iterator it = actions_.find( entity );
    if( it != actions_.end() )
    {
        T_Actions::iterator itAction = it->second.find( &action );
        if( itAction != it->second.end() )
        {
            if( itAction->second == selectedItem_ )
                ClearSelection();
            delete itAction->second;
            it->second.erase( itAction );
            if( it->second.empty() )
                NotifyDeleted( *it->first );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    T_EntityActions::iterator it = actions_.find( &entity );
    if( it != actions_.end() )
    {
        for( T_Actions::iterator itAction = it->second.begin(); itAction != it->second.end(); ++itAction )
        {
            if( itAction->second == selectedItem_ )
                ClearSelection();
            delete itAction->second;
        }
        actions_.erase( it );
        T_Entities::iterator itEntity = std::find( entities_.begin(), entities_.end(), &entity );
        entities_.erase( itEntity );
        canvas()->resize( canvas()->width(), canvas()->height() - rowHeight_ );
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::Update
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::Update()
{
    for( T_Entities::iterator it = entities_.begin(); it != entities_.end(); ++it )
    {
        const int row = std::distance( entities_.begin(), it );
        T_Actions& actions = actions_[ *it ];
        for( T_Actions::iterator itAction = actions.begin(); itAction != actions.end(); ++itAction )
        {
            TimelineActionItem& item = *itAction->second;
            item.setY( row * rowHeight_ );
            item.setSize( item.width(), rowHeight_ );
            item.setVisible( !filter_ || filter_->Allows( *itAction->first ) );
            item.Update();
        }
    }
    marker_->Update();
    canvas()->setAllChanged();
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::Select
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::Select( const QPoint& point )
{
    grabPoint_ = point;
    QCanvasItemList list = canvas()->collisions( grabPoint_ );
    if( list.empty() )
        ClearSelection();
    else
        for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
            if( *it != selectedItem_ )
            {
                TimelineItem_ABC* item = dynamic_cast< TimelineItem_ABC* >( *it );
                SetSelected( *item );
                break;
            }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMousePressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::contentsMousePressEvent( QMouseEvent* event )
{
    setFocus();
    if( ( event->button() & Qt::LeftButton ) == Qt::LeftButton )
        Select( event->pos() );
    if( selectedItem_ )
        ensureVisible( int( selectedItem_->x() ), int( selectedItem_->y() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMouseMoveEvent
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineView::contentsMouseMoveEvent( QMouseEvent* event )
{
    if( event->state() & Qt::LeftButton )
    {
        if( !selectedItem_ )
            Select( grabPoint_ );
        if( selectedItem_ )
        {
            selectedItem_->Move( event->pos().x() - grabPoint_.x() );
            ensureVisible( int( selectedItem_->x() ), int( selectedItem_->y() ) );
            grabPoint_ = event->pos();
            setCursor( QCursor::sizeHorCursor );
        }
    }
    Update();
    QCanvasItemList list = canvas()->collisions( event->pos() );
    if( list.empty() )
        QToolTip::remove( this );
    else if( const TimelineItem_ABC* item = dynamic_cast< const TimelineItem_ABC* >( *list.begin() ) )
        item->DisplayToolTip( this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMouseReleaseEvent
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineView::contentsMouseReleaseEvent( QMouseEvent* )
{
    if( selectedItem_ == marker_ )
        marker_->CommitMove();
    setCursor( QCursor::arrowCursor );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsContextMenuEvent
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::contentsContextMenuEvent( QContextMenuEvent* event )
{
    if( !selectedItem_ )
        Select( event->pos() );
    QPopupMenu* menu = new QPopupMenu( this );
    if( selectedItem_ )
    {
        selectedItem_->DisplayContextMenu( menu );
        menu->insertSeparator();
    }
    menu->insertItem( tools::translate( "TimelineView", "Zoom In" ), &ruler_, SLOT( ZoomIn() ), Qt::Key_Plus );
    menu->insertItem( tools::translate( "TimelineView", "Zoom Out" ), &ruler_, SLOT( ZoomOut() ), Qt::Key_Minus );
    menu->popup( event->globalPos() );
    event->accept();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem_ && event->key() == Qt::Key_Delete )
        selectedItem_->Delete();
    else if( event->key() == Qt::Key_Plus )
        ruler_.ZoomIn();
    else if( event->key() == Qt::Key_Minus )
        ruler_.ZoomOut();
    else if( selectedItem_ && ( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ) )
    {
        const short sign = event->key() == Qt::Key_Left ? -1 : 1;
        const long seconds = ( event->state() & Qt::ShiftButton ) ? 3600 * 24 : 3600;
        selectedItem_->Move( ruler_.ConvertToPixels( sign * seconds ) );
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::ClearSelection
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::ClearSelection()
{
    if( selectedItem_ )
    {
        selectedItem_->setSelected( false );
        controllers_.actions_.Select( (const actions::Action_ABC*)0 );
    }
    selectedItem_ = 0;

}

// -----------------------------------------------------------------------------
// Name: TimelineView::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::SetSelected( TimelineItem_ABC& item )
{
    if( selectedItem_ )
        selectedItem_->setSelected( false );
    item.setSelected( true );
    selectedItem_ = &item;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifySelected
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void TimelineView::NotifySelected( const actions::Action_ABC* action )
{
    const kernel::Entity_ABC* entity = 0;
    if( action )
        if( const ActionTasker* tasker = action->Retrieve< ActionTasker >() )
            entity = tasker->GetTasker();

    T_EntityActions::iterator it = actions_.find( entity );
    if( it != actions_.end() )
    {
        T_Actions::iterator itAction = it->second.find( action );
        if( itAction != it->second.end() )
        {
            if( selectedItem_ != itAction->second )
                SetSelected( *itAction->second );
            return;
        }
    }
    ClearSelection();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::setContentsPos
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::setContentsPos( int x, int y )
{
    blockSignals( true );
    QCanvasView::setContentsPos( x, y );
    blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::SetFilter
// Created: SBO 2010-05-06
// -----------------------------------------------------------------------------
void TimelineView::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    if( filter_ != &filter )
    {
        filter_ = &filter;
        Update();
    }
}
