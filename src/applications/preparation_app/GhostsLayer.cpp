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
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/StandardModel.h"

// -----------------------------------------------------------------------------
// Name: GhostsLayer constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostsLayer::GhostsLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile )
    : gui::EntityLayer< kernel::Ghost_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Ghosts )
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
    return ( dnd::HasData< kernel::Ghost_ABC >( event ) && selectedGhost_ ) ||
           ( dnd::HasData< GhostPositions >( event ) && selectedGhost_ ) ||
           ( dnd::HasData< kernel::GhostPrototype >( event ) && ( selectedAutomat_ || selectedFormation_ ) ) ||
             dnd::HasData< kernel::AgentType >( event ) || dnd::HasData< kernel::AutomatType >( event );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleEnterDragEvent
// Created: JSR 2012-12-05
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    const kernel::Entity_ABC* entity = dnd::FindSafeData< kernel::Ghost_ABC >( event );
    if( entity )
        oldPosition_ = entity->Retrieve< kernel::Positions >()->GetPosition();
    return gui::EntityLayer< kernel::Ghost_ABC >::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleMoveDragEvent
// Created: ABR 2011-10-26
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( GhostPositions* positions = dnd::FindData< GhostPositions >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            const geometry::Point2f newPosition =  point + draggingOffset_.ToVector();
            if( world_.IsInside( newPosition) )
            {
                positions->Move( newPosition );
                draggingPoint_ = point;
            }
        }
        return true;
    }
    kernel::Entity_ABC* entity = dnd::FindSafeData< kernel::Ghost_ABC >( event );
    if( entity )
    {
        if( kernel::Moveable_ABC* position = dynamic_cast< kernel::Moveable_ABC* >( entity->Retrieve< kernel::Positions >() ) )
        {
            position->Move( point );
            draggingPoint_ = point;
        }
        return true;
    }

    bool found = false;
    if( dnd::HasData< kernel::AgentType >( event ) || dnd::HasData< kernel::AutomatType >( event ) )
    {
        Pick( point );
        // Move ghost on map
        for( unsigned int i = 0; i < entities_.size() && !found; ++i )
            if( IsInSelection( *entities_[ i ] ) )
            {
                highLightedGhost_ = static_cast< const kernel::Ghost_ABC* >( entities_[ i ] );
                if( ( dnd::HasData< kernel::AgentType >( event ) && highLightedGhost_->GetGhostType() == eGhostType_Agent ) ||
                    ( dnd::HasData< kernel::AutomatType >( event ) && highLightedGhost_->GetGhostType() == eGhostType_Automat ) )
                {
                    highLightedGhost_->OverFly( controllers_.actions_ );
                    found = true;
                }
                else
                    highLightedGhost_ = 0;
            }
    }
    if( !found )
    {
        highLightedGhost_ = 0;
        controllers_.actions_.OverFly( point );
    }
    return ( dnd::HasData< kernel::GhostPrototype >( event ) && ( selectedAutomat_ || selectedFormation_ ) ) ||
           ( dnd::HasData< kernel::AgentType >( event ) && ( highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Agent ||
                                                                                    selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Agent ) ) ||
           ( dnd::HasData< kernel::AutomatType >( event ) && ( highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Automat ||
                                                                                    selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Automat ) );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleLeaveDragEvent
// Created: JSR 2012-12-05
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    if( oldPosition_.IsZero() )
        return false;
    if( selectedGhost_ )
    {
        kernel::Moveable_ABC* position = dynamic_cast< kernel::Moveable_ABC* >( selectedGhost_.ConstCast()->Retrieve< kernel::Positions >() );
        if( position )
        {
            position->Move( oldPosition_ );
            oldPosition_ = geometry::Point2f();
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleDropEvent
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    // Move from ODB
    if( dnd::HasData< kernel::Ghost_ABC >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        kernel::Positions& positions = selectedGhost_.ConstCast()->Get< kernel::Positions >();
        if( kernel::Moveable_ABC* moveable = dynamic_cast< kernel::Moveable_ABC* >( &positions ) )
        {
            moveable->Move( point );
            return true;
        }
        return false;
    }
    // Move from map
    if( dnd::HasData< GhostPositions >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    // Create ghost from creation panel
    if( const kernel::GhostPrototype* prototype = dnd::FindData< kernel::GhostPrototype >( event ) )
    {
        kernel::Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( !selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        if( !selectedEntity )
            return false;
        model_.ghosts_->Create( *selectedEntity, *prototype, point );
        return true;
    }
    // Create agent from ghost
    if( const kernel::AgentType* agentType = dnd::FindData< kernel::AgentType >( event ) )
    {
        const bool fromSelection = selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Agent;
        const bool fromHighLight = highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Agent;
        if( !fromSelection && !fromHighLight )
            return false;

        kernel::Ghost_ABC* currentGhost = ( fromHighLight ) ? highLightedGhost_.ConstCast() : selectedGhost_.ConstCast();
        assert( currentGhost && currentGhost->Retrieve< kernel::Positions >() );
        const geometry::Point2f position = ( fromHighLight ) ? currentGhost->Retrieve< kernel::Positions >()->GetPosition() : point;
        kernel::Agent_ABC* agent = model_.agents_->CreateAgent( *currentGhost, *agentType, position );
        delete currentGhost;
        agent->Select( controllers_.actions_ );
        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( agent );
        agent->MultipleSelect( controllers_.actions_, list );
        selectedGhost_ = 0;
        highLightedGhost_ = 0;
        return true;
    }
    // Create automat from ghost
    if( const kernel::AutomatType* automatType = dnd::FindData< kernel::AutomatType >( event ) )
    {
        const bool fromSelection = selectedGhost_ && selectedGhost_->GetGhostType() == eGhostType_Automat;
        const bool fromHighLight = highLightedGhost_ && highLightedGhost_->GetGhostType() == eGhostType_Automat;
        if( !fromSelection && !fromHighLight )
            return false;

        kernel::Ghost_ABC* currentGhost = ( fromHighLight ) ? highLightedGhost_.ConstCast() : selectedGhost_.ConstCast();
        assert( currentGhost && currentGhost->Retrieve< kernel::Positions >() );
        const geometry::Point2f position = ( fromHighLight ) ? currentGhost->Retrieve< kernel::Positions >()->GetPosition() : point;
        kernel::Automat_ABC* automat = model_.agents_->CreateAutomatInsteadOf( *currentGhost, *automatType, position );
        delete currentGhost;
        automat->Select( controllers_.actions_ );
        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( automat );
        automat->MultipleSelect( controllers_.actions_, list );
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
        delete selectedGhost_;
        selectedGhost_ = 0;
        highLightedGhost_ = 0;
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
    if( ( event->button() & Qt::LeftButton ) != 0 && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
        if( const GhostPositions* pos = static_cast< const GhostPositions* >( selectedGhost_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            dnd::CreateDragObject( pos, dummy_.get() );
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::BeforeMultiSelection
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::BeforeMultiSelection()
{
    selectedGhost_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void GhostsLayer::MultipleSelect( const std::vector< const kernel::Ghost_ABC* >& elements )
{
    selectedGhost_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void GhostsLayer::MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements )
{
    selectedAutomat_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void GhostsLayer::MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements )
{
    selectedFormation_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::IsEligibleForDrag
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::IsEligibleForDrag()
{
    return selectedGhost_ && IsInSelection( *selectedGhost_ );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::SetAlpha
// Created: ABR 2011-10-21
// -----------------------------------------------------------------------------
void GhostsLayer::SetAlpha( float alpha )
{
    Layer::SetAlpha( alpha );
}
