// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLocationList.h"
#include "ParamLocation.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLocation.h"
#include "gaming/ActionParameterLocationList.h"
#include "gaming/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ListParameter( parent, parameter.GetName().c_str(), controller, parameter.IsOptional() )
    , converter_( converter )
    , parameter_( parameter )
    , layer_( layer )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList destructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::~ParamLocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamLocationList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterLocationList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamLocationList::CreateElement()
{
    return new ParamLocation( OrderParameter( tools::translate( "ParamLocationList", "Location %1" ).arg( ++count_ ).ascii(), "location", false ), layer_, converter_ );
}
