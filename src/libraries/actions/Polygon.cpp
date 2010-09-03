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
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Common::MsgLocation& message )
    : Location( parameter, converter, message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Location( parameter, converter, xis )
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
void Polygon::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->mutable_polygon()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: Polygon::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().has_polygon() )
        message.mutable_value()->clear_polygon();
}

namespace
{
    struct AsnSerializer : public ParameterVisitor_ABC
    {
        explicit AsnSerializer( Common::MsgLocation& message ) : message_( &message ) {}
        virtual void Visit( const Location& param )
        {
            param.CommitTo( *message_ );
        }
        Common::MsgLocation* message_;
    };
}

// -----------------------------------------------------------------------------
// Name: Path::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::CommitTo( Common::MsgLocation& message ) const
{
    message.set_type( Common::MsgLocation::polygon );
    AsnSerializer serializer( message );
    Accept( serializer );
}
