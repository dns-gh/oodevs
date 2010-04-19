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
#include "protocol/Protocol.h"

using namespace Common;

using namespace kernel;
using namespace actions;
using namespace parameters;

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
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgPoint& message )
    : Location( parameter, converter, message.location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Location( parameter, converter, xis )
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
void Point::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    message.mutable_value()->mutable_point()->mutable_location()->set_type( MsgLocation_Geometry_point );    // enforce initialisation of parameter to force his type
    if( IsSet() )
        Location::CommitTo( *message.mutable_value()->mutable_point()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Point::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Point::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_point() )
        message.mutable_value()->clear_point();
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Point::CommitTo( Common::MsgCoordLatLong& message ) const
{
    Common::MsgLocation loc;
    Location::CommitTo( loc );
    message = loc.coordinates().elem( 0 );
    Location::Clean( loc );
}
