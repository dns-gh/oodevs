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
#include "TimelineRuler.h"
#include "TimelineEntityItem.h"
#include "TimelineActionItem.h"
#include "gaming/Action_ABC.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TimelineEditor constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEditor::TimelineEditor( QWidget* parent, QCanvas* canvas, Controllers& controllers, ActionsModel& actions )
    : QCanvasView( canvas, parent )
    , controllers_( controllers )
    , actions_( actions )
    , selectedAction_( 0 )
{
    lines_.push_back( new TimelineRuler( canvas, this ) );
    controllers_.Register( *this );

    updateTimer_ = new QTimer( this );
    connect( updateTimer_, SIGNAL( timeout()), SLOT( Update() ) );
    updateTimer_->start(0);
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEditor::~TimelineEditor()
{
    controllers_.Remove( *this );
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
// Name: TimelineEditor::mousePressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::mousePressEvent( QMouseEvent* event )
{
    setFocus();
    ClearSelection();
    grabPoint_ = ConvertToContent( event->pos() );
    QCanvasItemList list = canvas()->collisions( grabPoint_ );
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        if( TimelineActionItem* item = dynamic_cast< TimelineActionItem* >( *it ) )
        {
            SetSelected( *item ); // $$$$ SBO 2007-07-02: rotate through selection...
            break;;
        }
        else if( dynamic_cast< TimelineEntityItem* >( *it ) )
        {
            // $$$$ SBO 2007-07-05: ?
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::mouseMoveEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::mouseMoveEvent( QMouseEvent* event )
{
    if( !selectedAction_ )
        return;
    if( event->state() | Qt::LeftButton )
    {
        const QPoint position = ConvertToContent( event->pos() );
        selectedAction_->Shift( position.x() - grabPoint_.x() );
        ensureVisible( selectedAction_->x(), selectedAction_->y() );
        grabPoint_ = position;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::keyPressEvent( QKeyEvent* event )
{
    if( !selectedAction_ )
        return;
    if( event->key() == Qt::Key_Delete )
        selectedAction_->setVisible( false );
    else if( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right )
    {
        const short sign = event->key() == Qt::Key_Left ? -1 : 1;
        selectedAction_->Shift( sign * ( ( event->state() & Qt::ShiftButton ) ? 100 : 5 ) ); // $$$$ SBO 2007-07-05: ruler.getPageStep / ruler.getTickStep
    }
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
    selectedAction_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::SetSelected( TimelineActionItem& item )
{
    item.setSelected( true );
    selectedAction_ = &item;
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::ConvertToContent
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
QPoint TimelineEditor::ConvertToContent( const QPoint& point ) const
{
    return inverseWorldMatrix().map( viewportToContents( point ) );
}
