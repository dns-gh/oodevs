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
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace actions;
using namespace parameters;

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
// Name: Quantity constructor
// Created: JSR 2010-04-14
// -----------------------------------------------------------------------------
Quantity::Quantity( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< int >( parameter, xml::attribute< int >( xis, "value" ) )
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
void Quantity::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    message.mutable_value();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        message.mutable_value()->set_quantity( GetValue() );
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
    xos << attribute( "value", GetValue() );
}
