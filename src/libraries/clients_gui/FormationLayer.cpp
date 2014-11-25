// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FormationLayer.h"
#include "moc_FormationLayer.cpp"
#include "GLOptions.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers,
                                GLView_ABC& view,
                                ColorStrategy_ABC& strategy,
                                const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Formation_ABC >( controllers, view, strategy, profile, eLayerTypes_Formations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifyActivated
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void FormationLayer::NotifyActivated( const kernel::Formation_ABC& formation )
{
    if( tools::HasSubordinate( formation, []( const kernel::Entity_ABC& entity ){ return entity.GetTypeName() == kernel::Agent_ABC::typeName_; } ) )
        EntityLayer< kernel::Formation_ABC >::NotifyActivated( formation );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::ContextMenu
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
void FormationLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Formation_ABC& formation = static_cast< const Formation_ABC& >( entity );
    if( !formation.IsAnAggregatedSubordinate() && view_.GetActiveOptions().IsAggregated( formation ) )
        controllers_.actions_.ContextMenu( this, formation, entity, point, where );
}
