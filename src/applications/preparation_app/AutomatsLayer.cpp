// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AutomatsLayer.h"
#include "clients_gui/AggregatedPositions.h"
#include "clients_gui/DragAndDropHelpers.h"

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( kernel::Controllers& controllers,
                              gui::GLView_ABC& view,
                              gui::ColorStrategy_ABC& strategy,
                              const kernel::Profile_ABC& profile )
    : gui::AutomatsLayer( controllers, view, strategy, profile )
    , selectedAutomat_( controllers )
    , dummy_( new QWidget() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::BeforeMultiSelection
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
void AutomatsLayer::BeforeMultiSelection()
{
    gui::AutomatsLayer::BeforeMultiSelection();
    selectedAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::MultipleSelect
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
void AutomatsLayer::MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements )
{
    selectedAutomat_ = elements.size() == 1 ? elements.front() : 0;
    gui::AutomatsLayer::MultipleSelect( elements );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::CanDrop
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool AutomatsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return dnd::HasData< gui::AggregatedPositions >( event ) && selectedAutomat_;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleDropEvent
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& /*point*/ )
{
    auto positions = dnd::FindData< gui::AggregatedPositions >( event );
    if( positions && selectedAutomat_ )
    {
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleMoveDragEvent
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( auto positions = dnd::FindData< gui::AggregatedPositions >( event ) )
    {
        if( selectedAutomat_ && world_.IsInside( point ) && draggingPoint_.Distance( point ) >= 5.f * view_.Pixels( point ) )
        {
            positions->Move( point + draggingOffset_.ToVector() );
            draggingPoint_ = point;
        }
        return true;
    }
    return gui::AutomatsLayer::HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleMousePress
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( ( event->button() & Qt::LeftButton ) != 0 && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
    {
        if( const gui::AggregatedPositions* pos = static_cast< const gui::AggregatedPositions* >( selectedAutomat_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            dnd::CreateDragObject( pos, dummy_.get() );
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::IsEligibleForDrag
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool AutomatsLayer::IsEligibleForDrag() const
{
    return selectedAutomat_ && IsInSelection( *selectedAutomat_ );
}
