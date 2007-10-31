// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterBool.h"
#include "xeumeuleu/xml.h"

using namespace xml;

namespace
{
    bool ReadBool( xml::xistream& xis )
    {
        bool value;
        xis >> attribute( "value", value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterBool::ActionParameterBool( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< bool >( parameter, ReadBool( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterBool::ActionParameterBool( const kernel::OrderParameter& parameter, bool value )
    : ActionParameter< bool >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterBool::~ActionParameterBool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterBool::Serialize( xml::xostream& xos ) const
{
    ActionParameter< bool >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterBool::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_aBool;
    asn.value.u.aBool = GetValue();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterBool::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterBool::CommitTo( ASN1BOOL& asn ) const
{
    asn = GetValue();
}
