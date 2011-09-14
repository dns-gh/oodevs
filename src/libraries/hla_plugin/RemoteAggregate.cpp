// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAggregate.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "RemoteAgentListener_ABC.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::RemoteAggregate( const std::string& identifier, RemoteAgentListener_ABC& listener )
    : identifier_( identifier )
    , listener_  ( listener )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::~RemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote aggregate can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    if( identifier == "Spatial" )
    {
        Spatial spatial;
        spatial.Deserialize( deserializer );
        listener_.Moved( identifier_, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
    }
    else if( identifier == "ForceIdentifier" )
    {
        int8 force;
        deserializer >> force;
        listener_.SideChanged( identifier_, static_cast< rpr::ForceIdentifier >( force ) );
    }
    else if( identifier == "AggregateMarking" )
    {
        AggregateMarking marking;
        marking.Deserialize( deserializer );
        listener_.NameChanged( identifier_, marking.str() );
    }
}
