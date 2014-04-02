// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PathPoint.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PathPoint constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
PathPoint::PathPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathPoint destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
PathPoint::~PathPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathPoint::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathPoint::CommitTo( sword::CoordLatLong& message ) const
{
    sword::Location loc;
    Location::CommitTo( loc );
    message = loc.coordinates().elem( 0 );
    Location::Clean( loc );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PathPoint::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: PathPoint::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string PathPoint::SerializeType() const
{
    return "pathpoint";
}