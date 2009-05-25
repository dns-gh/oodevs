// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamMissionObjectiveList.h"
#include "ParamMissionObjective.h"
#include "actions/Action_ABC.h"
#include "actions/ObjectiveList.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamMissionObjectiveList::ParamMissionObjectiveList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const QDateTime& currentDate, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller )
    : ListParameter( parent, parameter.GetName().c_str(), controller, parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , currentDate_( currentDate )
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
void ParamMissionObjectiveList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ObjectiveList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList::CreateElement
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
Param_ABC* ParamMissionObjectiveList::CreateElement()
{
    return new ParamMissionObjective( kernel::OrderParameter( tools::translate( "ParamMissionObjectiveList", "Objective %1" ).arg( ++count_ ).ascii(), "objective", false ), layer_, currentDate_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: ParamMissionObjectiveList::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamMissionObjectiveList::IsOptional() const
{
    return parameter_.IsOptional();
}
