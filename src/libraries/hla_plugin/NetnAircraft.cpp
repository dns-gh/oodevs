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
#include "AttributesSerializer.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAircraft constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnAircraft::NetnAircraft( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& /*agent*/, const std::string& callsign, const std::string& uniqueIdentifier, const std::string& /*symbol*/ )
    : aggregate_ ( aggregate )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "UniqueID", UniqueId( uniqueIdentifier ) );
    attributes_->Register( "Callsign", UnicodeString( callsign ) );
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
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnAircraft::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void NetnAircraft::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
