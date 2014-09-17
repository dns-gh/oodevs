// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnHuman.h"
#include "Agent_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include "SerializationTools.h"
#include "AttributesSerializer.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnHuman constructor
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
NetnHuman::NetnHuman( std::unique_ptr< HlaObject_ABC > aggregate, Agent_ABC& /*agent*/, const std::string& callsign,
    const std::vector< char >& uniqueIdentifier, const std::string& /*symbol*/, FOM_Serializer_ABC& fomSerializer, const std::string& /*rtiId*/ )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( aggregate ) )
    , fomSerializer_( fomSerializer )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "UniqueID", UniqueId( uniqueIdentifier ) );
    attributes_->Register( "Callsign", UnicodeString( callsign ) );
}

// -----------------------------------------------------------------------------
// Name: NetnHuman destructor
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
NetnHuman::~NetnHuman( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::Serialize
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::Deserialize
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer_ABC& /*deserializer*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::GetIdentifier
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
const std::string& NetnHuman::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::Register
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::Unregister
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHuman::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnHuman::ResetAttributes()
{
    // NOTHING
}

Agent_ABC* const NetnHuman::GetAgent() const
{
	return 0;
}
