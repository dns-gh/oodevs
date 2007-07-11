// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPointList.h"
#include "ParamLocation.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPointList.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPointList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::ParamPointList( QObject* parent, const OrderParameter& parameter, gui::ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::~ParamPointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamPointList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterPointList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPointList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( true, false, false, false );
    return param;
}
