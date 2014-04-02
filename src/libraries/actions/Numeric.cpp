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

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Numeric constructor
// Created: ABR 2011-11-16
// -----------------------------------------------------------------------------
Numeric::Numeric( const kernel::OrderParameter& parameter )
    : Parameter< float >( parameter )
{
    SetValue( 0 );
    Set( false );
}

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
    if( IsSet() )
        xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Numeric::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Numeric::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.add_value()->set_areal( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Numeric::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Numeric::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.set_areal( GetValue() );
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

// -----------------------------------------------------------------------------
// Name: Numeric::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Numeric::SerializeType() const
{
    return "numeric";
}
