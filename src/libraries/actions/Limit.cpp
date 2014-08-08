// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Limit.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : Location( parameter, converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Line& line )
    : Location( parameter, converter, line.location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Limit::Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Limit::~Limit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Limit::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Location::CommitTo( *message.add_value()->mutable_limit()->mutable_location() );
}
// -----------------------------------------------------------------------------
// Name: Limit::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Limit::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        Location::CommitTo( *message.mutable_limit()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Limit::DisplayInToolTip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Limit::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: Limit::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Limit::SerializeType() const
{
    return "limit";
}

// -----------------------------------------------------------------------------
// Name: Limit::Accept
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void Limit::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
