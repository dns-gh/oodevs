// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentsLayer.h"
#include "ModelBuilder.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/AgentPositions.h"
#include "preparation/HierarchyTemplate.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/ValuedDragObject.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const Profile_ABC& profile )
    : gui::AgentsLayer  ( controllers, tools, strategy, view, profile )
    , model_            ( model )
    , modelBuilder_     ( modelBuilder )
    , selectedAgent_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , selectedTeam_     ( controllers )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::BeforeSelection
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::BeforeSelection()
{
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
    selectedTeam_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer::AfterSelection
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Agent_ABC& element )
{
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Automat_ABC& element )
{
    selectedAutomat_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Team_ABC& element )
{
    selectedTeam_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleEnterDragEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    return ( ValuedDragObject::Provides< const AgentPositions >   ( event ) && selectedAgent_ )
        || ( ValuedDragObject::Provides< const AgentType >        ( event ) && selectedAutomat_ )
        || ( ValuedDragObject::Provides< const AutomatType >      ( event ) && ( selectedFormation_ || selectedAutomat_ ) )
        || ( ValuedDragObject::Provides< const HierarchyTemplate >( event ) && IsValidTemplate( event ) );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsValidTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentsLayer::IsValidTemplate( QDragEnterEvent* event ) const
{
    if( !selectedFormation_ && !selectedTeam_ )
        return false;
    const HierarchyTemplate* droppedItem = ValuedDragObject::GetValue< const HierarchyTemplate >( event );
    if( ! droppedItem )
        return false;
    return selectedTeam_ ? droppedItem->IsCompatible( *selectedTeam_ )
                         : droppedItem->IsCompatible( *selectedFormation_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( AgentPositions* position = ValuedDragObject::GetValue< AgentPositions >( event ) )
    {
        if( !selectedAgent_ )
            return false;
        // if the events comes from the list or if far enough
        if( event->source() || position->GetPosition().Distance( point ) > 100 )
            position->Set( point );
        return true;
    }
    if( const AgentType* droppedItem = ValuedDragObject::GetValue< const AgentType >( event ) )
    {
        if( !selectedAutomat_ )
            return false;
        model_.agents_.CreateAgent( *selectedAutomat_.ConstCast(), *droppedItem, point );
        return true;
    }
    if( const AutomatType* droppedItem = ValuedDragObject::GetValue< const AutomatType >( event ) )
    {
        Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( ! selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        if( ! selectedEntity )
            return false;
        model_.agents_.CreateAutomat( *selectedEntity, *droppedItem, point );
        return true;
    }
    if( const HierarchyTemplate* droppedItem= ValuedDragObject::GetValue< const HierarchyTemplate >( event ) )
    {
        if( !selectedFormation_ && !selectedTeam_ )
            return false;
        Entity_ABC* superior = selectedFormation_ ? selectedFormation_.ConstCast() : (Entity_ABC*)selectedTeam_.ConstCast();
        droppedItem->Instanciate( *superior, point );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMousePress
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = gui::AgentsLayer::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        const AgentPositions* pos = static_cast< const AgentPositions* >( selectedAgent_->Retrieve< Positions >() );
        QDragObject* drag = new ValuedDragObject( pos );
        drag->drag();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool AgentsLayer::IsEligibleForDrag( const geometry::Point2f& point ) const
{
    return selectedAgent_ && IsInSelection( *selectedAgent_, point );
}
