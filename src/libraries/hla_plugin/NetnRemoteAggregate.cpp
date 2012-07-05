// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteAggregate.h"
#include "AttributesDeserializer.h"
#include "ObjectListenerComposite.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>
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
// Name: NetnRemoteAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregate::NetnRemoteAggregate( std::auto_ptr< HlaObject_ABC > aggregate, const std::string& identifier )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( aggregate )
    , attributes_( new AttributesDeserializer( identifier, *listeners_ ) )
{
    attributes_->Register( "Callsign", boost::bind( &ReadCallsign, _1, _2, _3 ) );
    attributes_->Register( "UniqueID", boost::bind( &ReadUniqueId, _1, _2, _3 ) );
    attributes_->Register( "EmbeddedUnitList", boost::bind( &ReadEmbeddedUnitList, _1, _2, _3 ) );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregate::~NetnRemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote aggregate can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    aggregate_->Deserialize( identifier, deserializer );
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::SetIdentifier
// Created: AHC 2012-03-15
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::SetIdentifier( const std::string& id )
{
    aggregate_->SetIdentifier( id );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& NetnRemoteAggregate::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}
