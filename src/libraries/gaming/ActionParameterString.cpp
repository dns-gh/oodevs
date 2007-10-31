// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterString.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterString constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterString::ActionParameterString( const kernel::OrderParameter& parameter, const std::string& value )
    : ActionParameter< QString >( parameter, value.c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterString constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
ActionParameterString::ActionParameterString( const kernel::OrderParameter& parameter, const ASN1VisibleString& asn )
    : ActionParameter< QString >( parameter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterString constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterString::ActionParameterString( const OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< QString >( parameter, attribute< std::string >( xis, "value" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterString destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
ActionParameterString::~ActionParameterString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterString::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterString::CommitTo( ASN1VisibleString& asn ) const
{
    const QString value = GetValue();
    asn = value.ascii();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterString::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void ActionParameterString::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", GetValue().ascii() );
}
