// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PointList.h"
#include "Point.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter )
    : LocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgPointList& message )
    : LocationList( parameter, converter, message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : LocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::~PointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PointList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        LocationList::CommitTo< Common::MsgPointList >( *message.mutable_value()->mutable_pointlist() );
}

// -----------------------------------------------------------------------------
// Name: PointList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PointList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_pointlist() )
        message.mutable_value()->clear_pointlist();
}
