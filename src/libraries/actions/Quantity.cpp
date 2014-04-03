// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Quantity.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Quantity constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Quantity::Quantity( const kernel::OrderParameter& parameter )
    : Parameter< int >( parameter, 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Quantity constructor
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Quantity::Quantity( const kernel::OrderParameter& parameter, int value )
    : Parameter< int >( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Quantity destructor
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Quantity::~Quantity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Quantity::CommitTo
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Quantity::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->set_quantity( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Quantity::CommitTo
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Quantity::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_quantity( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Quantity::CommitTo
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Quantity::CommitTo( T_Setter setter ) const
{
    setter( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Quantity::Accept
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Quantity::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Quantity::Serialize
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
void Quantity::Serialize( xml::xostream& xos ) const
{
    Parameter< int >::Serialize( xos );
    if( IsSet() )
        xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Quantity::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Quantity::SerializeType() const
{
    return "quantity";
}
