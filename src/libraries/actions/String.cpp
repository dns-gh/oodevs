// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "String.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: String constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
String::String( const kernel::OrderParameter& parameter, const std::string& value )
    : Parameter< QString >( parameter, value.c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: String constructor
// Created: SBO 2007-10-29
// -----------------------------------------------------------------------------
String::String( const kernel::OrderParameter& parameter, const ASN1VisibleString& asn )
    : Parameter< QString >( parameter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: String constructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
String::String( const OrderParameter& parameter, xml::xistream& xis )
    : Parameter< QString >( parameter, attribute< std::string >( xis, "value" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: String destructor
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
String::~String()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: String::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void String::CommitTo( ASN1VisibleString& asn ) const
{
    const QString value = GetValue();
    asn = value.isNull() ? "" : value.ascii();
}

// -----------------------------------------------------------------------------
// Name: String::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void String::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << attribute( "value", GetValue().ascii() );
}
