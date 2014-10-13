// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationsLayer.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/PopulationPrototype.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/PopulationPositions.h"

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( kernel::Controllers& controllers,
                                    gui::GLView_ABC& view,
                                    gui::ColorStrategy_ABC& strategy,
                                    Model& model,
                                    const kernel::Profile_ABC& profile )
    : gui::PopulationsLayer( controllers, view, strategy, profile )
    , model_             ( model )
    , selectedEntity_    ( controllers )
    , selectedPopulation_( controllers )
    , dummy_             ( new QWidget() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsLayer::~PopulationsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::CanDrop
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return ( dnd::HasData< kernel::PopulationPrototype >( event ) && selectedEntity_ ) ||
           ( dnd::HasData< PopulationPositions >( event ) && selectedPopulation_ ) ||
           ( dnd::HasData< kernel::Population_ABC >( event ) && selectedPopulation_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleEnterDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    if( dnd::HasData< kernel::Population_ABC >( event ) && selectedPopulation_ )
        if( const kernel::Positions* position = selectedPopulation_->Retrieve< kernel::Positions >() )
            oldPosition_ = position->GetPosition( true );
    return gui::PopulationsLayer::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( selectedPopulation_ && ( dnd::HasData< kernel::Population_ABC >( event ) || dnd::HasData< PopulationPositions >( event ) ) )
    {
        if( kernel::Moveable_ABC* positions = static_cast< kernel::Moveable_ABC* >( selectedPopulation_.ConstCast()->Retrieve< kernel::Positions >() ) )
        {
            const geometry::Point2f newPosition = point + draggingOffset_.ToVector();
            if( draggingPoint_.Distance( point ) >= 5.f * view_.Pixels( point ) && world_.IsInside( newPosition ) )
            {
                positions->Move( newPosition );
                draggingPoint_ = point;
            }
        }
        return true;
    }
    return gui::PopulationsLayer::HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleDropEvent
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( selectedEntity_ )
    {
        if( const kernel::PopulationPrototype* droppedItem = dnd::FindData< kernel::PopulationPrototype >( event ) )
        {
            model_.agents_->CreatePopulation( *selectedEntity_.ConstCast(), *(droppedItem->type_), droppedItem->number_, point );
            return true;
        }
    }
    else if( selectedPopulation_ )
    {
        bool hasPopulation = dnd::HasData< kernel::Population_ABC >( event );
        if( hasPopulation )
            oldPosition_ = geometry::Point2f();
        if( hasPopulation || dnd::HasData< PopulationPositions >( event ) )
        {
            draggingPoint_.Set( 0, 0 );
            draggingOffset_.Set( 0, 0 );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleLeaveDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    if( selectedPopulation_ && !oldPosition_.IsZero() )
    {
        static_cast< kernel::Moveable_ABC* >( selectedPopulation_.ConstCast()->Retrieve< kernel::Positions >() )->Move( oldPosition_ );
        oldPosition_ = geometry::Point2f();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleKeyPress
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete && selectedPopulation_ )
    {
        delete static_cast< const kernel::Population_ABC* >( selectedPopulation_ );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::BeforeMultiSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::BeforeMultiSelection()
{
    selectedEntity_ = 0;
    selectedPopulation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void PopulationsLayer::MultipleSelect( const std::vector< const kernel::Entity_ABC* >& elements )
{
    selectedEntity_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void PopulationsLayer::MultipleSelect( const std::vector< const kernel::Population_ABC* >& elements )
{
    selectedPopulation_ = elements.size() == 1 ? elements.front() : 0;
}

// $$$$ AGE 2007-07-03: ^c^v...

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMousePress
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( ( event->button() & Qt::LeftButton ) != 0 && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
        if( const PopulationPositions* pos = static_cast< const PopulationPositions* >( selectedPopulation_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            dnd::CreateDragObject( pos, dummy_.get() );
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::IsEligibleForDrag()
{
    return selectedPopulation_ && IsInSelection( *selectedPopulation_ );
}
