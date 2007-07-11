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
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPolygon.h"

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
void ParamPolygon::CommitTo( ActionParameterContainer_ABC& action ) const
{
    action.AddParameter( *CreateParameter< ActionParameterPolygon >() );
}
