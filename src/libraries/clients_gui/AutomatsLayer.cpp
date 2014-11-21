// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AutomatsLayer.h"
#include "moc_AutomatsLayer.cpp"
#include "GLOptions.h"
#include "Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers,
                              GLView_ABC& tools,
                              ColorStrategy_ABC& strategy,
                              const Profile_ABC& profile )
    : EntityLayer< Automat_ABC >( controllers, tools, strategy, profile, eLayerTypes_Automats )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifyActivated
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifyActivated( const kernel::Automat_ABC& automat )
{
    if( tools::HasSubordinate( automat, []( const kernel::Entity_ABC& ){ return true; } ) )
        EntityLayer< kernel::Automat_ABC >::NotifyActivated( automat );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::ContextMenu
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void AutomatsLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Automat_ABC& automat = static_cast< const Automat_ABC& >( selectable );
    if( !entity.IsAnAggregatedSubordinate() && view_.GetActiveOptions().IsAggregated( entity ) )
        controllers_.actions_.ContextMenu( this, automat, entity, point, where );
}
