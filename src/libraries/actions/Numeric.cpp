// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Numeric.h"
#include "ParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Numeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Numeric::Numeric( const kernel::OrderParameter& parameter, float value )
    : Parameter< float >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Numeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Numeric::Numeric( const kernel::OrderParameter& parameter, xml::xistream& xis )
: Parameter< float >( parameter, xml::attribute< float >( xis, "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Numeric destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Numeric::~Numeric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Numeric::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Numeric::Serialize( xml::xostream& xos ) const
{
    Parameter< float >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Numeric::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Numeric::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_aReal;
    if( IsSet() )
        asn.value.u.aReal = GetValue();
}

// -----------------------------------------------------------------------------
// Name: Numeric::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Numeric::CommitTo( ASN1REAL& asn ) const
{
    asn = GetValue();
}

// -----------------------------------------------------------------------------
// Name: Numeric::Accept
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Numeric::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
