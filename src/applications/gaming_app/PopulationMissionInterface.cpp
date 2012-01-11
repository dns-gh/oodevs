// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationMissionInterface.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MissionType.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller,
                                                        actions::gui::InterfaceBuilder_ABC& builder, ActionsModel& model )
    : actions::gui::MissionInterface_ABC( parent, mission, entity, controller )
    , model_( model )
    , mission_( mission )
{
    builder.BuildAll( *this, entity, mission );
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
PopulationMissionInterface::~PopulationMissionInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void PopulationMissionInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
    CommitTo( *action );
    model_.Publish( *action );
}
