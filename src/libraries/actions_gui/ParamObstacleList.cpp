//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObstacleList.h"
#include "ParamObstacle.h"
#include "actions/Action_ABC.h"
#include "actions/EngineerConstructionList.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"
#include "clients_gui/ValuedListItem.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller )
    : ListParameter( parent, parameter.GetName().c_str(), actions, parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , controller_( controller )
    , objectTypes_( objectTypes )
    , layer_( layer )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::~ParamObstacleList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CommitTo
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ParamObstacleList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::EngineerConstructionList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CreateElement
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Param_ABC* ParamObstacleList::CreateElement()
{
    return new ParamObstacle(this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "obstacle", false ), objectTypes_, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamObstacleList::IsOptional() const
{
    return parameter_.IsOptional();
}
