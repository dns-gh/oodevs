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
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, Controllers& controllers,
                                                        actions::gui::InterfaceBuilder_ABC& builder, ActionsModel& model, const tools::ExerciseConfig& config, const kernel::Time_ABC& simulation )
    : actions::gui::MissionInterface_ABC( parent, mission, entity, controllers, config, simulation, "crowds-mission-sheets-directory" )
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
    if( IsPlanned() )
        emit PlannedMission( *action, GetPlanningDate() );
    else
        model_.Publish( *action, 0 );
}
