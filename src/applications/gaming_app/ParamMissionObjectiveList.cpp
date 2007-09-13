// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamMissionObjectiveList.h"
#include "ParamMissionObjective.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterObjectiveList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjectiveList::ParamMissionObjectiveList( QObject* parent, const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const Simulation& simulation, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , simulation_( simulation )
    , layer_( layer )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjectiveList::~ParamMissionObjectiveList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList::CommitTo
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamMissionObjectiveList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterObjectiveList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList::CreateElement
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Param_ABC* ParamMissionObjectiveList::CreateElement()
{
    return new ParamMissionObjective( OrderParameter( tr( "Objective %1" ).arg( ++count_ ), "objective", false ), layer_, simulation_, converter_ );
}
