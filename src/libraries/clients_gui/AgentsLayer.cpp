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
#include "AggregatedTools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                          const Profile_ABC& profile )
    : EntityLayer< Agent_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Agent )
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
void AgentsLayer::Select( const GraphicalEntity_ABC& selectable, bool control, bool shift )
{
    const kernel::Entity_ABC& entity = static_cast< const kernel::Entity_ABC& >( selectable );
    const Entity_ABC* superior = entity.Get< TacticalHierarchies >().GetSuperior();
    if( shift && superior )
    {
        controllers_.actions_.SetSelected( *superior, control );
        superior->Activate( controllers_.actions_ );
    }
    else
        controllers_.actions_.SetSelected( selectable, control );
}
