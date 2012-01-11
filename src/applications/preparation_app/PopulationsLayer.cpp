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
#include "clients_kernel/CommunicationHierarchies.h"
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
                                    gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EditorProxy< gui::PopulationsLayer >( controllers, tools, strategy, view, profile, filter )
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
        || ( gui::ValuedDragObject::Provides< const PopulationPositions >( event ) && selectedPopulation_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( selectedPopulation_ )
        if( PopulationPositions* positions = gui::ValuedDragObject::GetValue< PopulationPositions >( event ) )
        {
            if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
            {
                positions->Move( point + draggingOffset_.ToVector() );
                draggingPoint_ = point;
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
        if( const PopulationPrototype* droppedItem = gui::ValuedDragObject::GetValue< const kernel::PopulationPrototype >( event ) )
        {
            model_.agents_.CreatePopulation( *selectedEntity_.ConstCast(), *(droppedItem->type_), droppedItem->number_, point );
            return true;
        }
    }
    else if( selectedPopulation_ )
    {
        if( PopulationPositions* positions = gui::ValuedDragObject::GetValue< PopulationPositions >( event ) )
        {
            draggingPoint_.Set( 0, 0 );
            draggingOffset_.Set( 0, 0 );
            return true;
        }
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
// Name: PopulationsLayer::Select
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::Select( const kernel::Entity_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::Select
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::Select( const kernel::Population_ABC& element )
{
    selectedPopulation_ = &element;
}

// $$$$ AGE 2007-07-03: ^c^v...

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMousePress
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = gui::PopulationsLayer::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        if( const PopulationPositions* pos = static_cast< const PopulationPositions* >( selectedPopulation_->Retrieve< Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            Q3DragObject* drag = new gui::ValuedDragObject( pos, dummy_.get() );
            drag->dragMove();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::IsEligibleForDrag( const geometry::Point2f& point )
{
    return selectedPopulation_ && IsInSelection( *selectedPopulation_, point );
}
