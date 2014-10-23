// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FormationLayer.h"
#include "clients_gui/AggregatedPositions.h"
#include "clients_gui/DragAndDropHelpers.h"

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers,
                                gui::GLView_ABC& view,
                                gui::ColorStrategy_ABC& strategy,
                                const kernel::Profile_ABC& profile )
    : gui::FormationLayer( controllers, view, strategy, profile )
    , selectedFormation_( controllers )
    , dummy_( new QWidget() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer destructor
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::BeforeMultiSelection
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
void FormationLayer::BeforeMultiSelection()
{
    gui::FormationLayer::BeforeMultiSelection();
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::MultipleSelect
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
void FormationLayer::MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements )
{
    selectedFormation_ = elements.size() == 1 ? elements.front() : 0;
    gui::FormationLayer::MultipleSelect( elements );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::CanDrop
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool FormationLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return dnd::HasData< gui::AggregatedPositions >( event ) && selectedFormation_;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HandleDropEvent
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool FormationLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& /*point*/ )
{
    gui::AggregatedPositions* positions = dnd::FindData< gui::AggregatedPositions >( event );
    if( positions && selectedFormation_ )
    {
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HandleMoveDragEvent
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool FormationLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    gui::AggregatedPositions* positions = dnd::FindData< gui::AggregatedPositions >( event );
    if( positions )
    {
        if( selectedFormation_ && world_.IsInside( point ) && draggingPoint_.Distance( point ) >= 5.f * view_.Pixels( point ) )
        {
            positions->Move( point + draggingOffset_.ToVector() );
            draggingPoint_ = point;
        }
        return true;
    }
    return gui::FormationLayer::HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HandleMousePress
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool FormationLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( ( event->button() & Qt::LeftButton ) != 0 && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
    {
        if( const gui::AggregatedPositions* pos = static_cast< const gui::AggregatedPositions* >( selectedFormation_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            dnd::CreateDragObject( pos, dummy_.get() );
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::IsEligibleForDrag
// Created: JSR 2013-05-29
// -----------------------------------------------------------------------------
bool FormationLayer::IsEligibleForDrag() const
{
    return selectedFormation_ && IsInSelection( *selectedFormation_ );
}
