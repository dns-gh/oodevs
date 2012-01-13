// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomateMissionInterface.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MissionType.h"
#include "gaming/AutomatDecisions.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller,
                                                    actions::gui::InterfaceBuilder_ABC& builder, ActionsModel& model )
    : actions::gui::MissionInterface_ABC( parent, mission, entity, controller )
    , model_ ( model )
    , mission_ ( mission )
{
    builder.BuildAll( *this, entity, mission );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
AutomateMissionInterface::~AutomateMissionInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void AutomateMissionInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
    CommitTo( *action );
    if( const AutomatDecisions* decisions = GetEntity().Retrieve< AutomatDecisions >() )
        if( ! decisions->IsEmbraye() )
            decisions->Engage();
    model_.Publish( *action );
}
