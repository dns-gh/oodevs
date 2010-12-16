// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "String.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
String::String( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< QString >( parameter, xis.attribute< std::string >( "value" ).c_str() )
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
void String::CommitTo( std::string& message ) const
{
    const QString value = GetValue();
    message = value.isNull() ? "" : value.ascii();
}

// -----------------------------------------------------------------------------
// Name: String::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void String::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.mutable_value()->Add()->set_acharstr( GetValue().ascii() );
}
// -----------------------------------------------------------------------------
// Name: String::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void String::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_acharstr( GetValue().ascii() );
}

// -----------------------------------------------------------------------------
// Name: String::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void String::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << xml::attribute( "value", GetValue().ascii() );
}
