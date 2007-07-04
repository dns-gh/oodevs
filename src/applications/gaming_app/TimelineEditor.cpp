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
{
    overlayItems_.push_back( new TimelineRuler( canvas, this ) );
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
        line = new TimelineEntityItem( canvas(), this, action.GetEntity() );
        line->SetYOffset( items_.size() - 1 );
        overlayItems_.push_back( line );
    }
    line->NotifyCreated( action );
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::NotifyUpdated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::NotifyUpdated( const Action_ABC& action )
{
    CIT_EntityItems it = items_.find( &action.GetEntity() );
    if( it != items_.end() )
        it->second->NotifyUpdated( action );
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
        it->second->NotifyDeleted( action );
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
        T_Items::iterator overlayIt = std::find( overlayItems_.begin(), overlayItems_.end(), it->second );
        if( overlayIt != overlayItems_.end() )
            overlayItems_.erase( overlayIt );
        // $$$$ SBO 2007-07-04: delete it->second ?
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
    ClearSelection();
    QCanvasItemList list = canvas()->collisions( inverseWorldMatrix().map( viewportToContents( event->pos() ) ) );
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        if( TimelineActionItem* item = dynamic_cast< TimelineActionItem* >( *it ) )
        {
            SetSelected( *item ); // $$$$ SBO 2007-07-02: rotate through selection...
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::mouseReleaseEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::mouseReleaseEvent( QMouseEvent* event )
{

}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::mouseMoveEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::mouseMoveEvent( QMouseEvent* event )
{

}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::keyPressEvent( QKeyEvent* event )
{

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
}

// -----------------------------------------------------------------------------
// Name: TimelineEditor::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEditor::SetSelected( QCanvasItem& item )
{
    item.setSelected( true );
}
