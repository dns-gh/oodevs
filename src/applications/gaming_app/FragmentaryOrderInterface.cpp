// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FragmentaryOrderInterface.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrderType.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Entity_ABC& entity, const FragOrderType& fragOrder, Controllers& controllers,
                                                      actions::gui::InterfaceBuilder_ABC& builder, ActionsModel& model, const tools::ExerciseConfig& config, const kernel::Time_ABC& simulation )
    : actions::gui::MissionInterface_ABC( parent, fragOrder, entity, controllers, config, simulation, "fragorders-mission-sheets-directory" )
    , model_( model )
    , fragOrder_( fragOrder )
{
    builder.BuildAll( *this, entity, fragOrder );
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::~FragmentaryOrderInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), fragOrder_ );
    CommitTo( *action );
    if( IsPlanned() )
        emit PlannedMission( *action, GetPlanningDate() );
    else
        model_.Publish( *action, 0 );
}
