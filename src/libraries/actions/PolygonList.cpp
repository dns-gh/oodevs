// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PolygonList.h"
#include "Polygon.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter )
    : LocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgPolygonList& message )
    : LocationList( parameter, converter, message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : LocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::~PolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PolygonList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    message.mutable_value()->mutable_polygonlist();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        LocationList::CommitTo< Common::MsgPolygonList >( *message.mutable_value()->mutable_polygonlist() );
}

// -----------------------------------------------------------------------------
// Name: PolygonList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PolygonList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_polygonlist() )
        message.mutable_value()->clear_polygonlist();
}
