// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Enumeration.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Enumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Enumeration::Enumeration( const OrderParameter& parameter, xml::xistream& xis )
    : Parameter< std::string >( parameter )
    , value_( parameter.GetValue( xml::attribute< unsigned long >( xis, "value" ) ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Enumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Enumeration::Enumeration( const OrderParameter& parameter, unsigned int value )
    : Parameter< std::string >( parameter )
    , value_( parameter.GetValue( value ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Enumeration destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Enumeration::~Enumeration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Enumeration::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Enumeration::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << attribute( "value", value_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Enumeration::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Enumeration::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_enumeration;
    if( IsSet() )
        asn.value.u.enumeration = value_.GetId();
}
