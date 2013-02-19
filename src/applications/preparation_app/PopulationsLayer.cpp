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
#include "clients_kernel/PopulationPrototype.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/PopulationPositions.h"
#include "clients_gui/ValuedDragObject.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                                    gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile )
    : gui::PopulationsLayer( controllers, tools, strategy, view, profile )
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
    return ( gui::ValuedDragObject::Provides< const PopulationPrototype >( event ) && selectedEntity_ )
        || ( gui::ValuedDragObject::Provides< const PopulationPositions >( event ) && selectedPopulation_ )
        || ( gui::ValuedDragObject::Provides< const Entity_ABC >( event ) && selectedPopulation_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleEnterDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    if( kernel::Population_ABC* entity = dynamic_cast< kernel::Population_ABC* >( gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) ) )
    {
        PopulationPositions* position = static_cast< PopulationPositions* >( entity->Retrieve< kernel::Positions >() );
        oldPosition_ = position->GetPosition( true );
    }
    return gui::PopulationsLayer::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( selectedPopulation_ )
    {
        PopulationPositions* positions = gui::ValuedDragObject::GetValue< PopulationPositions >( event );
        if( !positions )
        {
            if( Entity_ABC* entity = gui::ValuedDragObject::GetValue< Entity_ABC >( event ) )
                positions = static_cast< PopulationPositions* >( selectedPopulation_.ConstCast()->Retrieve< Positions >() );
        }
        if( positions )
        {
            if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
            {
                const geometry::Point2f newPosition = point + draggingOffset_.ToVector();
                if( world_.IsInside( newPosition ) )
                {
                    positions->Move( newPosition );
                    draggingPoint_ = point;
                }
            }
            return true;
        }
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
        if( const PopulationPrototype* droppedItem = gui::ValuedDragObject::GetValue< const kernel::PopulationPrototype >( event ) )
        {
            model_.agents_.CreatePopulation( *selectedEntity_.ConstCast(), *(droppedItem->type_), droppedItem->number_, point );
            return true;
        }
    }
    else if( selectedPopulation_ )
    {
        PopulationPositions* positions = gui::ValuedDragObject::GetValue< PopulationPositions >( event );
        if( !positions )
        {
            if( kernel::Population_ABC* entity = dynamic_cast< kernel::Population_ABC* >( gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) ) )
            {
                positions = static_cast< PopulationPositions* >( selectedPopulation_.ConstCast()->Retrieve< Positions >() );
                oldPosition_ = geometry::Point2f();
            }
        }
        if( positions )
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
        static_cast< PopulationPositions* >( selectedPopulation_.ConstCast()->Retrieve< kernel::Positions >() )->Move( oldPosition_ );
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
        delete (const Population_ABC*)selectedPopulation_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::BeforeSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::BeforeSelection()
{
    selectedEntity_ = 0;
    selectedPopulation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::AfterSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::AfterSelection()
{
    // NOTHING
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
    if( ( event->button() & Qt::LeftButton ) != 0 && event->buttons() != Qt::NoButton && IsEligibleForDrag( point ) )
        if( const PopulationPositions* pos = static_cast< const PopulationPositions* >( selectedPopulation_->Retrieve< Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            Q3DragObject* drag = new gui::ValuedDragObject( pos, dummy_.get() );
            drag->dragMove();
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::IsEligibleForDrag( const geometry::Point2f& point )
{
    return selectedPopulation_ && IsInSelection( *selectedPopulation_, point );
}
