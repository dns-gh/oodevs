// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnGroundVehicle.h"
#include "Agent_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include "SerializationTools.h"
#include "AttributesSerializer.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle constructor
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
NetnGroundVehicle::NetnGroundVehicle( std::unique_ptr< HlaObject_ABC > aggregate, Agent_ABC& /*agent*/, const std::string& callsign,
    const std::vector< char >& uniqueIdentifier, const std::string& /*symbol*/, FOM_Serializer_ABC& fomSerializer, const std::string& /*rtiId*/ )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( aggregate ) )
    , fomSerializer_( fomSerializer )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "UniqueID", UniqueId( uniqueIdentifier ) );
    attributes_->Register( "Callsign", UnicodeString( callsign ) );
    attributes_->Register( "EmbeddedUnitList", VectorWrapper< UniqueId >( std::vector< UniqueId >() ) );

}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle destructor
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
NetnGroundVehicle::~NetnGroundVehicle( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::Serialize
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void NetnGroundVehicle::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::Deserialize
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void NetnGroundVehicle::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer_ABC& /*deserializer*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::GetIdentifier
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
const std::string& NetnGroundVehicle::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::Register
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void NetnGroundVehicle::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::Unregister
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void NetnGroundVehicle::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::EmbarkmentChanged
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void NetnGroundVehicle::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnGroundVehicle::EmbarkmentChanged
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void NetnGroundVehicle::ResetAttributes()
{
    // NOTHING
}

Agent_ABC* const NetnGroundVehicle::GetAgent() const
{
	return 0;
}
