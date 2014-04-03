// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Polygon.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Polygon::Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : Location( parameter, converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::Location& message )
    : Location( parameter, converter, message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::~Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.add_value()->mutable_area()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Polygon::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_area()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::CommitTo( sword::Location& message ) const
{
    message.set_type( sword::Location::polygon );
    Location::CommitTo( message );
}

// -----------------------------------------------------------------------------
// Name: Polygon::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Polygon::SerializeType() const
{
    return "polygon";
}

// -----------------------------------------------------------------------------
// Name: Polygon::Accept
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void Polygon::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
