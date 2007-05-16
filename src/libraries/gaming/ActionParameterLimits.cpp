// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimits.h"
#include "ActionParameterLimit.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line1, const ASN1T_Line& line2 )
    : ActionParameter< QString >( parameter )
{
    AddParameter( *new ActionParameterLimit( tools::translate( "ActionParameterLimits", "Limit 1" ), converter, line1 ) );
    AddParameter( *new ActionParameterLimit( tools::translate( "ActionParameterLimits", "Limit 2" ), converter, line2 ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimits::ActionParameterLimits( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterLimits::ReadLimit, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLimits::~ActionParameterLimits()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimits::ReadLimit
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterLimits::ReadLimit( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterLimit( converter, xis ) );
}
