// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPolygonList.h"
#include "ParamLocation.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPolygonList.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPolygonList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::ParamPolygonList( QObject* parent, const OrderParameter& parameter, gui::ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::~ParamPolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamPolygonList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterPolygonList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPolygonList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( false, false, true, false );
    return param;
}
