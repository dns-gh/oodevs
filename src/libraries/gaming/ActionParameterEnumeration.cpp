// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterEnumeration.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::ActionParameterEnumeration( const OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< std::string >( parameter )
    , value_( parameter.GetValue( xml::attribute< unsigned long >( xis, "value" ) ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::ActionParameterEnumeration( const OrderParameter& parameter, unsigned int value )
    : ActionParameter< std::string >( parameter )
    , value_( parameter.GetValue( value ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::~ActionParameterEnumeration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterEnumeration::Serialize( xml::xostream& xos ) const
{
    ActionParameter< std::string >::Serialize( xos );
    xos << attribute( "value", value_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterEnumeration::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_enumeration;
    if( IsSet() )
        asn.value.u.enumeration = value_.GetId();
}
