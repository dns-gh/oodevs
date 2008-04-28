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
#include "TimelineEntityItem.h"
#include "TimelineActionItem.h"
#include "TimelineRuler.h"
#include "gaming/Action_ABC.h"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"
#include "icons.h"
#include <qpainter.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimelineView constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineView::TimelineView( QWidget* parent, QCanvas* canvas, Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, TimelineRuler& ruler )
    : QCanvasView  ( canvas, parent )
    , controllers_ ( controllers )
    , model_       ( model )
    , ruler_       ( ruler )
    , selectedItem_( 0 )
{
    // initialize some elements needed in action tooltips
    QMimeSourceFactory::defaultFactory()->setPixmap( "mission", MAKE_PIXMAP( mission ) );

    viewport()->setMouseTracking( true );
    new TimelineMarker( canvas, scheduler, controllers_, ruler_ );
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
    T_EntityLine& line = entityLines_[ &action.GetEntity() ];
    if( line.second == 0 )
    {
        line.first = new TimelineEntityItem( canvas(), ruler_, controllers_, action.GetEntity() );
        line.first->MoveAfter( lines_.empty() ? 0 : lines_.back() );
        lines_.push_back( line.first );
        canvas()->resize( canvas()->width(), canvas()->height() + line.first->height() );
    }
    line.first->AddAction( action );
    ++line.second;
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const Action_ABC& action )
{
    T_EntityLines::iterator it = entityLines_.find( &action.GetEntity() );
    if( it != entityLines_.end() )
    {
        T_EntityLine& line = it->second;
        line.first->RemoveAction( action );
        if( --line.second == 0 )
            NotifyDeleted( action.GetEntity() );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    T_EntityLines::iterator it = entityLines_.find( &entity );
    if( it != entityLines_.end() )
    {
        T_Lines::iterator itLine = std::find( lines_.begin(), lines_.end(), it->second.first );
        if( itLine != lines_.end() )
            lines_.erase( itLine );
        const unsigned int height = it->second.first->height();
        delete it->second.first;
        entityLines_.erase( it );
        canvas()->resize( canvas()->width(), canvas()->height() - height );
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::Update
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::Update()
{
    for( T_Lines::const_iterator it = lines_.begin(); it != lines_.end(); ++it )
        (*it)->Update();
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
    {
        ClearSelection();
        Update();
        return;
    }
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        if( *it != selectedItem_ )
        {
            TimelineItem_ABC* item = dynamic_cast< TimelineItem_ABC* >( *it );
            ClearSelection();
            SetSelected( *item );
            Update();
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMousePressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::contentsMousePressEvent( QMouseEvent* event )
{
    setFocus();
    Select( event->pos() );
    if( selectedItem_ )
        ensureVisible( selectedItem_->x(), selectedItem_->y() );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMouseMoveEvent
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineView::contentsMouseMoveEvent( QMouseEvent* event )
{
    if( !selectedItem_ )
        Select( grabPoint_ );
    if( selectedItem_ && ( event->state() & Qt::LeftButton ) )
    {
        selectedItem_->Shift( event->pos().x() - grabPoint_.x() );
        ensureVisible( selectedItem_->x(), selectedItem_->y() );
        grabPoint_ = event->pos();
        setCursor( QCursor::sizeHorCursor );
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
    if( selectedItem_ )
    {
        selectedItem_->DisplayContextMenu( this, event->globalPos() );
        event->accept();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem_ && event->key() == Qt::Key_Delete )
        selectedItem_->setVisible( false ); // $$$$ SBO 2008-04-23: real delete
    else if( event->key() == Qt::Key_Plus )
        ruler_.ZoomIn();
    else if( event->key() == Qt::Key_Minus )
        ruler_.ZoomOut();
    else if( selectedItem_ && ( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ) )
    {
        const short sign = event->key() == Qt::Key_Left ? -1 : 1;
        const long seconds = ( event->state() & Qt::ShiftButton ) ? 3600 * 24 : 3600;
        selectedItem_->Shift( ruler_.ConvertToPixels( sign * seconds ) );
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::ClearSelection
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::ClearSelection()
{
    QCanvasItemList list = canvas()->allItems();
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        (*it)->setSelected( false );
    selectedItem_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::SetSelected( TimelineItem_ABC& item )
{
    item.setSelected( true );
    selectedItem_ = &item;
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
