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
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Bool constructor
// Created: ABR 2011-11-16
// -----------------------------------------------------------------------------
Bool::Bool( const kernel::OrderParameter& parameter )
    : Parameter< bool >( parameter )
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
    if( IsSet() )
        xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Bool::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->set_booleanvalue( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Bool::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_booleanvalue( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Bool::CommitTo( T_Setter setter ) const
{
    setter( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Bool::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Bool::SerializeType() const
{
    return "boolean";
}

// -----------------------------------------------------------------------------
// Name: Bool::Accept
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void Bool::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
