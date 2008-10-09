//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObstacleList.h"
#include "ParamObstacle.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/OrderParameter.h"
#include "actions/Action_ABC.h"
#include "actions/ObstacleList.h"
#include "gaming/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( QObject* parent, const OrderParameter& parameter, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& actions, Controller& controller )
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
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ObstacleList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CreateElement
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Param_ABC* ParamObstacleList::CreateElement()
{
    return new ParamObstacle(this, OrderParameter( tools::translate( "ParamObstacleList", "Obstacle %1" ).arg( ++count_ ).ascii(), "obstacle", false ), objectTypes_, layer_, converter_, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamObstacleList::IsOptional() const
{
    return parameter_.IsOptional();
}
