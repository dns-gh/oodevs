// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentsLayer.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "gaming/Attributes.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( kernel::Controllers& controllers,
                          gui::GLView_ABC& view,
                          gui::ColorStrategy_ABC& strategy,
                          const kernel::Profile_ABC& profile,
                          actions::ActionsModel& actionsModel,
                          const kernel::Time_ABC& simulation )
    : gui::EntityLayer< kernel::Agent_ABC >( controllers, view, strategy, profile, eLayerTypes_Agents )
    , selected_( controllers )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , displayDestroyedUnits_( true )
{
    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    controllers_.options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::CanDrop
// Created: JSR 2013-02-28
// -----------------------------------------------------------------------------
bool AgentsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return selected_ && ( dnd::HasData< const kernel::AgentType >( event ) );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: JSR 2013-02-28
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( !selected_ )
        return false;
    if( const kernel::AgentType* droppedItem = dnd::FindData< kernel::AgentType >( event ) )
    {
        RequestCreation( point, *droppedItem );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifySelectionChanged
// Created: JSR 2013-02-28
// -----------------------------------------------------------------------------
void AgentsLayer::NotifySelectionChanged( const std::vector< const kernel::Agent_ABC* >& elements )
{
    selected_ = elements.size() == 1 ? elements.front() : 0;
    gui::EntityLayer< kernel::Agent_ABC >::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::RequestCreation
// Created: JSR 2013-02-28
// -----------------------------------------------------------------------------
void AgentsLayer::RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type )
{
    const kernel::Entity_ABC* parent = selected_->Get< kernel::TacticalHierarchies >().GetSuperior();
    if( !parent )
        return;
    actionsModel_.PublishAgentCreationAction( type, point, *parent );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Draw
// Created: SLI 2014-07-30
// -----------------------------------------------------------------------------
void AgentsLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode )
{
    if( displayDestroyedUnits_ )
        gui::EntityLayer< kernel::Agent_ABC >::Draw( entity, viewport, pickingMode );
    else
    {
        const Attributes* attributes = entity.Retrieve< Attributes >();
        if( attributes && !attributes->bDead_ )
            gui::EntityLayer< kernel::Agent_ABC >::Draw( entity, viewport, pickingMode );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::OptionChanged
// Created: SLI 2014-07-31
// -----------------------------------------------------------------------------
void AgentsLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "DisplayDestroyedUnits" )
        displayDestroyedUnits_ = value.To< bool >();
}
