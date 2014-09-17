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
#include "FOM_Serializer_ABC.h"
#include "ObjectListenerComposite.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAircraft constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnAircraft::NetnAircraft( std::unique_ptr< HlaObject_ABC > aircraft, Agent_ABC& /*agent*/, const std::string& callsign,
        const std::vector< char >& uniqueIdentifier, const std::string& /*symbol*/, FOM_Serializer_ABC& fomSerializer, const std::string& rtiId )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( aircraft ) )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater( rtiId, *listeners_) )
    , callsign_( callsign )
    , uniqueId_( uniqueIdentifier )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
NetnAircraft::NetnAircraft( std::unique_ptr< HlaObject_ABC > aircraft, const std::string& identifier, FOM_Serializer_ABC& fomSerializer )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_    ( std::move( aircraft ) )
    , fomSerializer_( fomSerializer )
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
    attributesUpdater_->Register( "UniqueID", boost::bind( &FOM_Serializer_ABC::ReadUniqueId, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( uniqueId_ ) ), uniqueId_, fomSerializer_.GetUniqueIdSerializer() );
    attributesUpdater_->Register( "Callsign", boost::bind( &FOM_Serializer_ABC::ReadCallsign, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( callsign_ ) ), callsign_ );
    attributesUpdater_->Register( "EmbeddedUnitList", boost::bind( &FOM_Serializer_ABC::ReadEmbeddedUnitList, boost::ref( fomSerializer_ ), _1, _2, _3 ), 
        VariableArray< std::vector< char > >() , fomSerializer_.GetUniqueIdSerializer() );
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

Agent_ABC* const NetnAircraft::GetAgent() const
{
	return aggregate_->GetAgent();
}
