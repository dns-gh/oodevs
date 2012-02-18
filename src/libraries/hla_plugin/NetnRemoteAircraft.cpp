// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteAircraft.h"
#include "AttributesDeserializer.h"
#include "RemoteAgentListener_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadCallsign( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        UnicodeString callsign;
        callsign.Deserialize( deserializer );
        listener.CallsignChanged( identifier, callsign.str() );
    }
    void ReadUniqueId( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        UniqueId uniqueId;
        uniqueId.Deserialize( deserializer );
        listener.UniqueIdChanged( identifier, uniqueId.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAircraft::NetnRemoteAircraft( std::auto_ptr< HlaObject_ABC > vessel, RemoteAgentListener_ABC& listener, const std::string& identifier )
    : vessel_    ( vessel )
    , attributes_( new AttributesDeserializer( identifier, listener ) )
{
    attributes_->Register( "Callsign", boost::bind( &ReadCallsign, _1, _2, _3 ) );
    attributes_->Register( "UniqueID", boost::bind( &ReadUniqueId, _1, _2, _3 ) );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAircraft::~NetnRemoteAircraft()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAircraft::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote aircraft can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAircraft::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    vessel_->Deserialize( identifier, deserializer );
    attributes_->Deserialize( identifier.ToString(), deserializer );
}
