// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitMissionInterface.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/AutomatDecisions.h"

using namespace sword;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& entity, const kernel::MissionType& mission, ActionController& controller,
                                            actions::gui::InterfaceBuilder_ABC& builder, ActionsModel& model )
    : actions::gui::MissionInterface_ABC( parent, mission, entity, controller )
    , model_( model )
    , mission_( mission )
{
    builder.BuildAll( *this, entity, mission );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
UnitMissionInterface::~UnitMissionInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void UnitMissionInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
    CommitTo( *action );
    if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( GetEntity().Get< kernel::TacticalHierarchies >().GetSuperior() ) )
        if( const AutomatDecisions* decisions = automat->Retrieve< AutomatDecisions >() )
            if( decisions->IsEmbraye() )
                decisions->Disengage();
    model_.Publish( *action );
}
