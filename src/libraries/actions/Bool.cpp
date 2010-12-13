// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Bool.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Bool constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
Bool::Bool( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< bool >( parameter, xis.attribute< bool >( "value" ) )
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
    xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Bool::CommitTo( sword::MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    if( IsSet() )
       message.mutable_value()->Add()->set_booleanvalue( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Bool::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Bool::CommitTo( sword::MsgMissionParameter_Value& message ) const
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
