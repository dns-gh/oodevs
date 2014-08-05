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
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/XmlAdapter.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter,
                      kernel::Controller& controller )
    : Entity< kernel::Pathfind_ABC >( parameter, controller )
    , converter_( converter )
{
    // NOTHING
}

Itinerary::Itinerary( const kernel::OrderParameter& parameter,
                      const kernel::CoordinateConverter_ABC& converter,
                      const sword::Pathfind& pathfind,
                      const kernel::EntityResolver_ABC& resolver,
                      kernel::Controller& controller )
    : Entity< kernel::Pathfind_ABC >( parameter, resolver.FindPathfind( pathfind.id() ), controller )
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
    else if( IsSet() && GetValue() )
        *message.mutable_pathfind() = GetValue()->GetCreationMessage();
}

void Itinerary::Serialize( xml::xostream& xos ) const
{
    T_Parameter::Serialize( xos, kernel::XmlWriterAdapter( converter_ ) );
}

bool Itinerary::IsSet() const
{
    return pathfind_ || T_Parameter::IsSet();
}

void Itinerary::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

std::string Itinerary::SerializeType() const
{
    return "itinerary";
}
