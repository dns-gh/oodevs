// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPolygon.h"
#include "actions/Action_ABC.h"
#include "actions/Polygon.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPolygon constructor
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
ParamPolygon::ParamPolygon( const OrderParameter& parameter, gui::ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamLocation( parameter, layer, converter )
{
    SetShapeFilter( false, false, true, false );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygon destructor
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
ParamPolygon::~ParamPolygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygon::CommitTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ParamPolygon::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    actions::parameters::Polygon* pPolygon = CreateParameter< actions::parameters::Polygon >();
    if( pPolygon )
        action.AddParameter( *pPolygon );
}
