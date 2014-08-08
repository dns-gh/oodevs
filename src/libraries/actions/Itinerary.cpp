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
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/XmlAdapter.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
    , converter_( converter )
{
    // NOTHING
}

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter,
                      const sword::Pathfind& pathfind )
    : Parameter< QString >( parameter )
    , converter_( converter )
    , pathfind_( new sword::Pathfind( pathfind ) )
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
    if( pathfind_ )
        *message.mutable_pathfind() = *pathfind_;
}

void Itinerary::Serialize( xml::xostream& xos ) const
{
    T_Parameter::Serialize( xos, kernel::XmlWriterAdapter( converter_ ) );
}

bool Itinerary::IsSet() const
{
    return pathfind_;
}

void Itinerary::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

std::string Itinerary::SerializeType() const
{
    return "itinerary";
}

unsigned int Itinerary::GetId() const
{
    if( pathfind_ )
        return pathfind_->id();
    return 0;
}

const sword::Pathfind& Itinerary::GetPathfind() const
{
    if( pathfind_ )
        return *pathfind_;
    const static sword::Pathfind empty;
    return empty;
}
