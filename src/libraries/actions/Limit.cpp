// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Limit.h"

using namespace actions::parameters;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line )
    : Location( parameter, converter, line )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Location( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Limit::~Limit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Limit::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_line;
    asn.value.u.line = new ASN1T_Line();
    if( IsSet() )
        Location::CommitTo( *asn.value.u.line );
}

// -----------------------------------------------------------------------------
// Name: Limit::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Limit::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.line )
        Location::Clean( *asn.value.u.line );
    delete asn.value.u.line;
}

// -----------------------------------------------------------------------------
// Name: Limit::DisplayInToolTip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Limit::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}
