// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Bool.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Bool constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Bool::Bool( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< bool >( parameter, xml::attribute< bool >( xis, "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Bool constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Bool::Bool( const kernel::OrderParameter& parameter, bool value )
    : Parameter< bool >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Bool destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Bool::~Bool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Bool::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Bool::Serialize( xml::xostream& xos ) const
{
    Parameter< bool >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Bool::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_aBool;
    if( IsSet() )
        asn.value.u.aBool = GetValue();
}

// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Bool::CommitTo( ASN1BOOL& asn ) const
{
    asn = GetValue();
}
