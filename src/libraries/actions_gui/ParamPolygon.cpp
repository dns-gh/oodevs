// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPolygon.h"
#include "actions/Action_ABC.h"
#include "actions/Polygon.h"
#include "clients_kernel/Polygon.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPolygon constructor
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
ParamPolygon::ParamPolygon( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter )
    : ParamLocation( parameter, layer, converter )
{
    SetShapeFilter( false, false, true, false, false );
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
    Commit< actions::parameters::Polygon >( action );
}
