// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnAircraft.h"
#include "Agent_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include "SerializationTools.h"
#include "AttributesUpdater.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadCallsign( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UnicodeString& callsign )
    {
        callsign.Deserialize( deserializer );
        listener.CallsignChanged( identifier, callsign.str() );
    }
    void ReadUniqueId( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, UniqueId& uniqueId )
    {
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
            deserializer >> tmp;
            embeddedUnits[i]=tmp.str();
        }
        listener.EmbeddedUnitListChanged( identifier, embeddedUnits );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnAircraft::NetnAircraft( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& /*agent*/, const std::string& callsign, const std::string& uniqueIdentifier, const std::string& /*symbol*/ )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( aggregate )
    , attributesUpdater_( new AttributesUpdater(callsign, *listeners_) ) // TODO AHC check callsign
    , callsign_( callsign )
    , uniqueId_( uniqueIdentifier )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
NetnAircraft::NetnAircraft( std::auto_ptr< HlaObject_ABC > vessel, const std::string& identifier )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_    ( vessel )
    , attributesUpdater_( new AttributesUpdater(identifier, *listeners_) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnAircraft::~NetnAircraft()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Serialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void NetnAircraft::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributesUpdater_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void NetnAircraft::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    aggregate_->Deserialize( identifier, deserializer );
    attributesUpdater_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnAircraft::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnAircraft::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Attach
// Created: AHC 2012-03-09
// -----------------------------------------------------------------------------
void NetnAircraft::Attach( Agent_ABC* agent, unsigned long simId )
{
    aggregate_->Attach( agent, simId );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::RegisterAttributes
// Created: AHC 2012-03-12
// -----------------------------------------------------------------------------
void NetnAircraft::RegisterAttributes()
{
    attributesUpdater_->Register( "UniqueID", boost::bind( &ReadUniqueId, _1, _2, _3, boost::ref( uniqueId_ ) ), uniqueId_ );
    attributesUpdater_->Register( "Callsign", boost::bind( &ReadCallsign, _1, _2, _3, boost::ref( callsign_ ) ), callsign_ );
    attributesUpdater_->Register( "EmbeddedUnitList", boost::bind( &ReadEmbeddedUnitList, _1, _2, _3 ), Wrapper< std::vector< UniqueId > >( std::vector< UniqueId >() ) );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::SetIdentifier
// Created: AHC 2012-03-15
// -----------------------------------------------------------------------------
void NetnAircraft::SetIdentifier( const std::string& id )
{
    aggregate_->SetIdentifier( id );
    identifier_ = id;
    attributesUpdater_.reset( new AttributesUpdater(identifier_, *listeners_) );
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& NetnAircraft::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::GetIdentifier
// Created: AHC 2012-04-20
// -----------------------------------------------------------------------------
void NetnAircraft::ResetAttributes()
{
    attributesUpdater_->Update( "EmbeddedUnitList", Wrapper< std::vector< UniqueId > >( std::vector< UniqueId >() ) );
}
