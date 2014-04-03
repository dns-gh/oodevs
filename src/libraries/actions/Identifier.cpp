// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Identifier.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Identifier constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Identifier::Identifier( const kernel::OrderParameter& parameter )
    : Parameter< unsigned int >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Identifier constructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
Identifier::Identifier( const kernel::OrderParameter& parameter, unsigned int value )
    : Parameter< unsigned int >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Identifier destructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
Identifier::~Identifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Identifier::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void Identifier::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->set_identifier( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Identifier::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void Identifier::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_identifier( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Identifier::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void Identifier::CommitTo( T_Setter setter ) const
{
    setter( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Identifier::Accept
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void Identifier::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Identifier::Serialize
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void Identifier::Serialize( xml::xostream& xos ) const
{
    Parameter< unsigned int >::Serialize( xos );
    if( IsSet() )
        xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Identifier::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Identifier::SerializeType() const
{
    return "identifier";
}
