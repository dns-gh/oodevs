// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterNumeric.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::ActionParameterNumeric( const kernel::OrderParameter& parameter, float value )
    : ActionParameter< float >( parameter, value )
{
    // NOTHING
}

namespace
{
    float ReadValue( xml::xistream& xis )
    {
        float value;
        xis >> attribute( "value", value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::ActionParameterNumeric( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< float >( parameter, ReadValue( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::~ActionParameterNumeric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterNumeric::Serialize( xml::xostream& xos ) const
{
    ActionParameter< float >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterNumeric::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_aReal;
    asn.value.u.aReal = GetValue();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterNumeric::CommitTo( ASN1REAL& asn ) const
{
    asn = GetValue();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric::Accept
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterNumeric::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
