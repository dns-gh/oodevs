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
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: String constructor
// Created: ABR 2011-11-16
// -----------------------------------------------------------------------------
String::String( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

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
    if( IsSet() )
        message = GetValue().toStdString();
}

// -----------------------------------------------------------------------------
// Name: String::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void String::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->set_acharstr( GetValue().toStdString() );
}
// -----------------------------------------------------------------------------
// Name: String::CommitTo
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void String::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_acharstr( GetValue().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: String::Serialize
// Created: SBO 2007-10-31
// -----------------------------------------------------------------------------
void String::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    if( IsSet() )
        xos << xml::attribute( "value", GetValue().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: String::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string String::SerializeType() const
{
    return "string";
}

// -----------------------------------------------------------------------------
// Name: String::Accept
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void String::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
