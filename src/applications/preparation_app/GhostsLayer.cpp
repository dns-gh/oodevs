// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostsLayer.h"
#include "preparation/AgentsModel.h"
#include "preparation/GhostModel.h"
#include "preparation/GhostPositions.h"
#include "preparation/Model.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/Moveable_ABC.h"

#include "clients_gui/ValuedDragObject.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostsLayer constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostsLayer::GhostsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EditorProxy< gui::EntityLayer< kernel::Ghost_ABC > >( controllers, tools, strategy, view, profile, filter )
    , model_            ( model )
    , selectedGhost_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , highLightedGhost_ ( controllers )
    , dummy_            ( new QWidget() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer destructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostsLayer::~GhostsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::IsDropAllowed
// Created: ABR 2011-10-26
// -----------------------------------------------------------------------------
bool GhostsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return ( gui::ValuedDragObject::Provides< const GhostPrototype >( event ) && ( selectedAutomat_ || selectedFormation_ ) ) ||
           ( gui::ValuedDragObject::Provides< const GhostPositions >( event ) && selectedGhost_ ) ||
           ( gui::ValuedDragObject::Provides< const Entity_ABC >    ( event ) && selectedGhost_ ) ||
           ( gui::ValuedDragObject::Provides< const AgentType >     ( event ) ) ||
           ( gui::ValuedDragObject::Provides< const AutomatType >   ( event ) );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleMoveDragEvent
// Created: ABR 2011-10-26
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    // Move ghost on map
    if( GhostPositions* position = gui::ValuedDragObject::GetValue< GhostPositions >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            position->Move( point + draggingOffset_.ToVector() );
            draggingPoint_ = point;
        }
        return true;
    }
    bool found = false;
    for( unsigned i = 0; i < entities_.size() && !found; ++i )
        if( IsInSelection( *entities_[ i ], point ) )
        {
            highLightedGhost_ = static_cast< const Ghost_ABC* >( entities_[ i ] );
            if( ( gui::ValuedDragObject::Provides< const AgentType >( event ) && highLightedGhost_->GetGhostType() == eGhostType_Agent ) ||
                ( gui::ValuedDragObject::Provides< const AutomatType > ( event ) && highLightedGhost_->GetGhostType() == eGhostType_Automat ) )
            {
                highLightedGhost_->OverFly( controllers_.actions_ );
                found = true;
            }
            else
                highLightedGhost_ = 0;
        }
    if( !found )
    {
        highLightedGhost_ = 0;
        controllers_.actions_.OverFly( point );
    }
    return ( gui::ValuedDragObject::Provides< const GhostPrototype >( event ) && ( selectedAutomat_ || selectedFormation_ ) ) ||
           ( gui::ValuedDragObject::Provides< const Entity_ABC >    ( event ) && selectedGhost_ ) ||
           ( gui::ValuedDragObject::Provides< const AgentType >     ( event ) && ( highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Agent ||
                                                                                   selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Agent ) ) ||
           ( gui::ValuedDragObject::Provides< const AutomatType >   ( event ) && ( highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Automat ||
                                                                                   selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Automat ) );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleDropEvent
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    // Create ghost from creation panel
    if( const GhostPrototype* droppedItem = gui::ValuedDragObject::GetValue< const GhostPrototype >( event ) )
    {
        Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( !selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        if( !selectedEntity )
            return false;
        model_.ghosts_.Create( *selectedEntity, *droppedItem, point );
        return true;
    }
    // End of move ghost on map
    if( GhostPositions* position = gui::ValuedDragObject::GetValue< GhostPositions >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    // Move ghost from ODB
    if( const Entity_ABC* droppedItem = gui::ValuedDragObject::GetValue< const Entity_ABC >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        const kernel::Positions& positions = selectedGhost_->Get< Positions >();
        if( const kernel::Moveable_ABC* moveable = dynamic_cast< const kernel::Moveable_ABC* >( &positions ) )
        {
            const_cast< kernel::Moveable_ABC* >( moveable )->Move( point );
            return true;
        }
        return false;
    }
    // Create agent from ghost
    if( const AgentType* droppedItem = gui::ValuedDragObject::GetValue< const AgentType >( event ) )
    {
        const bool fromSelection = selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Agent;
        const bool fromHighLight = highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Agent;
        if( !fromSelection && !fromHighLight )
            return false;

        Ghost_ABC* currentGhost = ( fromHighLight ) ? highLightedGhost_.ConstCast() : selectedGhost_.ConstCast();
        assert( currentGhost && currentGhost->Retrieve< Positions >() );
        const geometry::Point2f position = ( fromHighLight ) ? currentGhost->Retrieve< Positions >()->GetPosition() : point;
        model_.agents_.CreateAgent( *currentGhost, *droppedItem, position );
        delete static_cast< const Ghost_ABC* >( currentGhost );
        selectedGhost_ = 0;
        highLightedGhost_ = 0;
        return true;
    }
    // Create automat from ghost
    if( const AutomatType* droppedItem = gui::ValuedDragObject::GetValue< const AutomatType >( event ) )
    {
        const bool fromSelection = selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Automat;
        const bool fromHighLight = highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Automat;
        if( !fromSelection && !fromHighLight )
            return false;

        Ghost_ABC* currentGhost = ( fromHighLight ) ? highLightedGhost_.ConstCast() : selectedGhost_.ConstCast();
        assert( currentGhost && currentGhost->Retrieve< Positions >() );
        const geometry::Point2f position = ( fromHighLight ) ? currentGhost->Retrieve< Positions >()->GetPosition() : point;
        model_.agents_.CreateAutomat( *currentGhost, *droppedItem, position );
        delete static_cast< const Ghost_ABC* >( currentGhost );
        selectedGhost_ = 0;
        highLightedGhost_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleKeyPress
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete && selectedGhost_ )
    {
        delete (const Ghost_ABC*)selectedGhost_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleMousePress
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = EntityLayer< kernel::Ghost_ABC >::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        if( const GhostPositions* pos = static_cast< const GhostPositions* >( selectedGhost_->Retrieve< Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            Q3DragObject* drag = new gui::ValuedDragObject( pos, dummy_ );
            drag->dragMove();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::BeforeSelection
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::BeforeSelection()
{
    selectedGhost_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::AfterSelection
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Ghost_ABC& element )
{
    selectedGhost_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Automat_ABC& element )
{
    selectedAutomat_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::IsEligibleForDrag
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::IsEligibleForDrag( const geometry::Point2f& point )
{
    return selectedGhost_ && IsInSelection( *selectedGhost_, point );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::SetAlpha
// Created: ABR 2011-10-21
// -----------------------------------------------------------------------------
void GhostsLayer::SetAlpha( float alpha )
{
    Layer_ABC::SetAlpha( alpha / 3.f );
}
