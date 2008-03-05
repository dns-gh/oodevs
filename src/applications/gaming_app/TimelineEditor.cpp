// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineEditor.h"
#include "moc_TimelineEditor.cpp"
#include "TimelineMarker.h"
#include "TimelineRuler.h"
#include "TimelineEntityItem.h"
#include "TimelineActionItem.h"
#include "gaming/Action_ABC.h"
#include "clients_kernel/Controllers.h"
#include "icons.h"
#include <qpainter.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimelineEditor constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEditor::TimelineEditor( QWidget* parent, QCanvas* canvas, Controllers& controllers, ActionsScheduler& scheduler )
    : QCanvasView( canvas, parent )
    , controllers_( controllers )
    , selectedItem_( 0 )
{
    // initialize some elements needed in action tooltips
    QMimeSourceFactory::defaultFactory()->setPixmap( "mission", MAKE_PIXMAP( mission ) );

    viewport()->setMouseTracking( true );
    new TimelineMarker( this, scheduler );
    lines_.push_back( new TimelineRuler( canvas, this ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEditor::~TimelineEditor()
{
    QMimeSourceFactory::defaultFactory()->setData( "mission", 0 );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::NotifyCreated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::NotifyCreated( const Action_ABC& action )
{
    TimelineEntityItem*& line = items_[ &action.GetEntity() ];
    if( !line )
    {
        line = new TimelineEntityItem( *this, lines_.empty() ? 0 : lines_.back(), controllers_, action.GetEntity() );
        lines_.push_back( line );
    }
    line->AddAction( action );
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::NotifyDeleted( const Action_ABC& action )
{
    CIT_EntityItems it = items_.find( &action.GetEntity() );
    if( it != items_.end() )
        it->second->RemoveAction( action );
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    T_EntityItems::iterator it = items_.find( &entity );
    if( it != items_.end() )
    {
        T_Lines::iterator itLine = std::find( lines_.begin(), lines_.end(), it->second );
        if( itLine != lines_.end() )
            lines_.erase( itLine );
        delete it->second;
        items_.erase( it );
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::Update
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::Update()
{
    for( T_Lines::const_iterator it = lines_.begin(); it != lines_.end(); ++it )
        (*it)->Update();
    canvas()->setAllChanged();
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::contentsMousePressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::contentsMousePressEvent( QMouseEvent* event )
{
    setFocus();
    grabPoint_ = event->pos();
    QCanvasItemList list = canvas()->collisions( grabPoint_ );
    if( list.empty() )
    {
        ClearSelection();
        Update();
        return;
    }
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        if( TimelineItem_ABC* item = dynamic_cast< TimelineItem_ABC* >( *it ) )
            if( item != selectedItem_ )
            {
                ClearSelection();
                SetSelected( *item );
                Update();
            }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::contentsMouseMoveEvent
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineEditor::contentsMouseMoveEvent( QMouseEvent* event )
{
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
    {
        QToolTip::remove( this );
        return;
    }
    if( const TimelineItem_ABC* item = dynamic_cast< const TimelineItem_ABC* >( *list.begin() ) )
        item->DisplayToolTip( this );
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::contentsMouseReleaseEvent
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineEditor::contentsMouseReleaseEvent( QMouseEvent* )
{
    setCursor( QCursor::arrowCursor );
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::keyPressEvent( QKeyEvent* event )
{
    if( !selectedItem_ )
        return;
    if( event->key() == Qt::Key_Delete )
        selectedItem_->setVisible( false );
    else if( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right )
    {
        const short sign = event->key() == Qt::Key_Left ? -1 : 1;
        selectedItem_->Shift( sign * ( ( event->state() & Qt::ShiftButton ) ? 100 : 5 ) ); // $$$$ SBO 2007-07-05: ruler.getPageStep / ruler.getTickStep
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::resizeEvent
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineEditor::resizeEvent( QResizeEvent* event )
{
    const int minHeight = lines_.empty() ? 0 : lines_.size() * lines_.back()->height(); // $$$$ SBO 2007-07-06: ruler + size * lineheight
    const QSize size = event->size();
    if( size.width() > canvas()->width() || size.height() > canvas()->height() )
        canvas()->resize( std::max( canvas()->width(), size.width() ), std::max( canvas()->height(), size.height() ) );
    else if( size.height() < canvas()->height() && size.height() > minHeight )
        canvas()->resize( canvas()->width(), size.height() );
    QCanvasView::resizeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::ClearSelection
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::ClearSelection()
{
    QCanvasItemList list = canvas()->allItems();
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        (*it)->setSelected( false );
    selectedItem_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::SetSelected( TimelineItem_ABC& item )
{
    item.setSelected( true );
    selectedItem_ = &item;
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::ConvertToContent
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
QPoint TimelineEditor::ConvertToContent( const QPoint& point ) const
{
    return inverseWorldMatrix().map( viewportToContents( point ) );
}
