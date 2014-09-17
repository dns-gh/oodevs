// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnSurfaceVessel.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "AttributesUpdater.h"
#include "ObjectListener_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "ObjectListenerComposite.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnSurfaceVessel::NetnSurfaceVessel( std::unique_ptr< HlaObject_ABC > vessel, Agent_ABC& /*agent*/, const std::string& callsign,
    const std::vector< char >& uniqueIdentifier, const std::string& /*symbol*/, FOM_Serializer_ABC& fomSerializer, const std::string& rtiId  )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( vessel ) )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater( rtiId, *listeners_) )
    , callsign_( callsign )
    , uniqueId_( uniqueIdentifier )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
NetnSurfaceVessel::NetnSurfaceVessel( std::unique_ptr< HlaObject_ABC > vessel, const std::string& identifier, FOM_Serializer_ABC& fomSerializer )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_    ( std::move( vessel ) )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater(identifier, *listeners_) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
NetnSurfaceVessel::~NetnSurfaceVessel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::Serialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributesUpdater_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    aggregate_->Deserialize( identifier, deserializer );
    attributesUpdater_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::RegisterAttributes
// Created: AHC 2012-03-08
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::RegisterAttributes()
{
    attributesUpdater_->Register( "UniqueID", boost::bind( &FOM_Serializer_ABC::ReadUniqueId, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( uniqueId_ ) ), uniqueId_, fomSerializer_.GetUniqueIdSerializer() );
    attributesUpdater_->Register( "Callsign", boost::bind( &FOM_Serializer_ABC::ReadCallsign, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( callsign_ ) ), callsign_ );
    attributesUpdater_->Register( "EmbeddedUnitList", boost::bind( &FOM_Serializer_ABC::ReadEmbeddedUnitList, boost::ref( fomSerializer_ ), _1, _2, _3 ), 
        VariableArray< std::vector< char > >(), fomSerializer_.GetUniqueIdSerializer() );
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::Attach
// Created: AHC 2012-03-09
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::Attach( Agent_ABC* agent, unsigned long simId )
{
    aggregate_->Attach( agent, simId );
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& NetnSurfaceVessel::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnSurfaceVessel::GetIdentifier
// Created: AHC 2012-04-20
// -----------------------------------------------------------------------------
void NetnSurfaceVessel::ResetAttributes()
{
    attributesUpdater_->Update( "EmbeddedUnitList", Wrapper< std::vector< UniqueId > >( std::vector< UniqueId >() ) );
}

Agent_ABC* const NetnSurfaceVessel::GetAgent() const
{
	return aggregate_->GetAgent();
}
