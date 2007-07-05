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
    , selectedItem_( 0 )
{
    new TimelineRuler( canvas, this );
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
        line = new TimelineEntityItem( *this, lines_.empty() ? 0 : lines_.back(), action.GetEntity() );
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
        delete it->second; // $$$$ SBO 2007-07-05: could be cleaned by Qt
        items_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::Update
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::Update()
{
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
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::mouseMoveEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::mouseMoveEvent( QMouseEvent* event )
{
    if( !selectedItem_ )
        return;
    if( event->state() | Qt::LeftButton )
    {
        const QPoint position = ConvertToContent( event->pos() );
        selectedItem_->moveBy( position.x() - grabPoint_.x(), 0 );
        grabPoint_ = position;
    }
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
    {
        // delete action
    }
    else if( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right )
    {
        const short sign = event->key() == Qt::Key_Left ? -1 : 1;
        selectedItem_->moveBy( sign * ( ( event->state() & Qt::ShiftButton ) ? 100 : 5 ), 0 ); // $$$$ SBO 2007-07-05: ruler.getPageStep / ruler.getTickStep
    }
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
void TimelineEditor::SetSelected( QCanvasItem& item )
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
