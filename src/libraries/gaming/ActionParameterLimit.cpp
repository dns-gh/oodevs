// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimit.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Line& line )
    : ActionParameterLocation( parameter, converter, line )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameterLocation( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const kernel::OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameterLocation( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::~ActionParameterLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimit::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_line;
    asn.value.u.line = new ASN1T_Line();
    ActionParameterLocation::CommitTo( *asn.value.u.line );
    asn.null_value = asn.value.u.line->coordinates.n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimit::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.line )
        ActionParameterLocation::Clean( *asn.value.u.line );
    delete asn.value.u.line;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::DisplayInToolTip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterLimit::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}
