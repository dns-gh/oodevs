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
#include "NetnRemoteAircraft.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include "ObjectListenerComposite.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadCallsign( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        UnicodeString callsign;
        callsign.Deserialize( deserializer );
        listener.CallsignChanged( identifier, callsign.str() );
    }
    void ReadUniqueId( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        UniqueId uniqueId;
        uniqueId.Deserialize( deserializer );
        listener.UniqueIdChanged( identifier, uniqueId.str() );
    }
    void ReadEmbeddedUnitList( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        std::vector< std::string > embeddedUnits;
        uint32 size;
        deserializer >> size;
        embeddedUnits.resize(size);
        for(uint32 i=0; i < size; ++i )
        {
            UniqueId tmp;
            tmp.Deserialize( deserializer );
            embeddedUnits[i]=tmp.str();
        }
        listener.EmbeddedUnitListChanged( identifier, embeddedUnits );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAircraft::NetnRemoteAircraft( std::auto_ptr< HlaObject_ABC > vessel, const std::string& identifier )
    : listeners_ ( new ObjectListenerComposite() )
    , vessel_    ( vessel )
    , attributes_( new AttributesDeserializer( identifier, *listeners_ ) )
{
    attributes_->Register( "Callsign", boost::bind( &ReadCallsign, _1, _2, _3 ) );
    attributes_->Register( "UniqueID", boost::bind( &ReadUniqueId, _1, _2, _3 ) );
    attributes_->Register( "EmbeddedUnitList", boost::bind( &ReadEmbeddedUnitList, _1, _2, _3 ) );
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
void NetnRemoteAircraft::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    vessel_->Deserialize( identifier, deserializer );
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::SetIdentifier
// Created: AHC 2012-03-15
// -----------------------------------------------------------------------------
void NetnRemoteAircraft::SetIdentifier( const std::string& id )
{
    vessel_->SetIdentifier( id );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& NetnRemoteAircraft::GetIdentifier( ) const
{
    return vessel_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnRemoteAircraft::Register( ObjectListener_ABC& listener )
{
    vessel_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAircraft::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnRemoteAircraft::Unregister( ObjectListener_ABC& listener )
{
    vessel_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}
