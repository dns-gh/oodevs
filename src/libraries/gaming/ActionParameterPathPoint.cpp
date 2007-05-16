// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPathPoint.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterPathPoint::ActionParameterPathPoint( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameterLocation( name, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterPathPoint::ActionParameterPathPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameterLocation( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterPathPoint::~ActionParameterPathPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterPathPoint::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
    ActionParameterLocation::DisplayInToolTip( displayer );
}
