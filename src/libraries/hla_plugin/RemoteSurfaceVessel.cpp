// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteSurfaceVessel.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "RemoteAgentListener_ABC.h"
#include "AttributesDeserializer.h"
#include "rpr/EntityType.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadSpatial( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        Spatial spatial;
        spatial.Deserialize( deserializer );
        listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
    }
    void ReadForceIdentifier( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        int8 force;
        deserializer >> force;
        listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( force ) );
    }
    void ReadMarking( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        Marking marking;
        marking.Deserialize( deserializer );
        listener.NameChanged( identifier, marking.str() );
    }
    void ReadEntityType( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        rpr::EntityType type;
        type.Deserialize( deserializer );
        listener.TypeChanged( identifier, type );
        listener.EquipmentUpdated( identifier, type, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteSurfaceVessel::RemoteSurfaceVessel( const std::string& identifier, RemoteAgentListener_ABC& listener )
    : attributes_( new AttributesDeserializer( identifier, listener ) )
{
    attributes_->Register( "Spatial"        , boost::bind( &ReadSpatial         , _1, _2, _3 ) );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier , _1, _2, _3 ) );
    attributes_->Register( "Marking"        , boost::bind( &ReadMarking         , _1, _2, _3 ) );
    attributes_->Register( "EntityType"     , boost::bind( &ReadEntityType      , _1, _2, _3 ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteSurfaceVessel::~RemoteSurfaceVessel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "RPR remote surface vessel can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}
