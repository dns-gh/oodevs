// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPathList.h"
#include "ParamPath.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "actions/Action_ABC.h"
#include "actions/PathList.h"
#include "gaming/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& entity, ActionController& controller )
    : ListParameter( parent, parameter.GetName().c_str(), controller, parameter.IsOptional() )
    , converter_( converter )
    , parameter_( parameter )
    , layer_( layer )
    , entity_( entity )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CommitTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ParamPathList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::PathList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPathList::CreateElement()
{
    return new ParamPath( this, OrderParameter( tools::translate( "ParamPathList", "Route %1" ).arg( ++count_ ).ascii(), "path", false ), layer_, converter_, entity_ );
}
