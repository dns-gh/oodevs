// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AgentsLayer.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EntityLayer< Agent_ABC >( controllers, tools, strategy, view, profile, filter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const Entity_ABC& entity, bool control, bool shift )
{
    const Entity_ABC* superior = entity.Get< CommunicationHierarchies >().GetSuperior();
    if( shift && superior )
    {
        controllers_.actions_.SetSelected( *superior, control );
        superior->Activate( controllers_.actions_ );
    }
    else
        controllers_.actions_.SetSelected( entity, control );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsAggregated
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
bool AgentsLayer::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::ShouldDisplay
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
bool AgentsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    return EntityLayer< Agent_ABC >::ShouldDisplay( entity ) && !IsAggregated( entity );
}
