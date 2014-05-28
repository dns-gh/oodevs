// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Itinerary.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/XmlAdapter.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
    , converter_( converter )
    , request_( new sword::PathfindRequest() )
{
    // NOTHING
}

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter,
                      const sword::PathfindRequest& request )
    : Parameter< QString >( parameter )
    , converter_( converter )
    , request_( new sword::PathfindRequest( request ) )
{
    // NOTHING
}

Itinerary::~Itinerary()
{
    // NOTHING
}

void Itinerary::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    CommitTo( *message.add_value() );
}

void Itinerary::CommitTo( sword::MissionParameter_Value& message ) const
{
    *message.mutable_pathfind_request() = *request_;
}

void Itinerary::Serialize( xml::xostream& xos ) const
{
    T_Parameter::Serialize( xos, kernel::XmlWriterAdapter( converter_ ) );
}

bool Itinerary::IsSet() const
{
    return true;
}

std::string Itinerary::SerializeType() const
{
    return "itinerary";
}
