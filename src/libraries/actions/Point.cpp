// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Point.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
Point::Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : Location( parameter, converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const sword::Point& message )
    : Location( parameter, converter, message.location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Point::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Location::CommitTo( *message.add_value()->mutable_point()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Point::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        Location::CommitTo( *message.mutable_point()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Point::CommitTo( sword::CoordLatLong& message ) const
{
    sword::Location loc;
    Location::CommitTo( loc );
    message = loc.coordinates().elem( 0 );
    Location::Clean( loc );
}

// -----------------------------------------------------------------------------
// Name: Point::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Point::SerializeType() const
{
    return "point";
}

// -----------------------------------------------------------------------------
// Name: Point::Accept
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void Point::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
