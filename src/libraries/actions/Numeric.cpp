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
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
void Numeric::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    message.mutable_value();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        message.mutable_value()->set_areal( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Numeric::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Numeric::CommitTo( T_Setter setter ) const
{
    setter( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Numeric::Accept
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void Numeric::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
