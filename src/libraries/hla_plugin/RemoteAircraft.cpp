// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAircraft.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "RemoteAgentListener_ABC.h"
#include "AttributesDeserializer.h"
#include "rpr/EntityType.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        Spatial spatial;
        spatial.Deserialize( deserializer );
        listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
    }
    void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        int8 force;
        deserializer >> force;
        listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( force ) );
    }
    void ReadMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        Marking marking;
        marking.Deserialize( deserializer );
        listener.NameChanged( identifier, marking.str() );
    }
    void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        rpr::EntityType type;
        type.Deserialize( deserializer );
        listener.TypeChanged( identifier, type );
        listener.EquipmentUpdated( identifier, type, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAircraft constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAircraft::RemoteAircraft( const std::string& identifier, RemoteAgentListener_ABC& listener )
    : attributes_( new AttributesDeserializer( identifier, listener ) )
{
    attributes_->Register( "Spatial"        , boost::bind( &ReadSpatial         , _1, _2, _3 ) );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier , _1, _2, _3 ) );
    attributes_->Register( "Marking"        , boost::bind( &ReadMarking         , _1, _2, _3 ) );
    attributes_->Register( "EntityType"     , boost::bind( &ReadEntityType      , _1, _2, _3 ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteAircraft destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAircraft::~RemoteAircraft()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAircraft::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAircraft::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "RPR remote surface vessel can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteAircraft::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAircraft::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}
