// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnAggregate.h"
#include "Agent_ABC.h"
#include "UnicodeString.h"
#include "UniqueId.h"
#include "SerializationTools.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include "AttributesUpdater.h"
#include "FOM_Serializer_ABC.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregate::NetnAggregate( std::unique_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& callsign,
        const std::vector< char >& uniqueIdentifier, const std::string& symbol, FOM_Serializer_ABC& fomSerializer, const std::string& rtiId  )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( aggregate ) )
    , agent_     ( &agent )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater( rtiId, *listeners_) )
    , callsign_  ( callsign )
    , uniqueId_  ( uniqueIdentifier )
    , symbol_    ( symbol )
    , mounted_   ( 0. )
    , status_    ( 1 )
{
    RegisterAttributes();
    agent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
NetnAggregate::NetnAggregate( std::unique_ptr< HlaObject_ABC > aggregate, const std::string& identifier, FOM_Serializer_ABC& fomSerializer )
    : listeners_ ( new ObjectListenerComposite() )
    , aggregate_ ( std::move( aggregate ) )
    , agent_ ( 0 )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater( identifier, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregate::~NetnAggregate()
{
    if( agent_ )
        agent_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributesUpdater_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    aggregate_->Deserialize( identifier, deserializer );
    attributesUpdater_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::SpatialChanged
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::SpatialChanged( double /*latitude*/, double /*longitude*/, float /*altitude*/, float /*speed*/, float /*direction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::FormationChanged
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::EquipmentChanged
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::EmbarkmentChanged
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::EmbarkmentChanged( bool mounted )
{
    attributesUpdater_->Update( "Mounted", Wrapper< double >( mounted ? 100. : 0. ) );
    attributesUpdater_->Update( "Status", Wrapper< int8_t >( mounted ? 2 : 1 ) );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnAggregate::Register( ObjectListener_ABC& listener )
{
    aggregate_->Register( listener );
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void NetnAggregate::Unregister( ObjectListener_ABC& listener )
{
    aggregate_->Unregister( listener );
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Attach
// Created: AHC 2012-03-09
// -----------------------------------------------------------------------------
void NetnAggregate::Attach( Agent_ABC* agent, unsigned long simId )
{
    aggregate_->Attach( agent, simId );
    if( agent_ )
        agent_->Unregister( *this );
    agent_ = agent;
    agent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::RegisterAttributes
// Created: AHC 2012-03-13
// -----------------------------------------------------------------------------
void NetnAggregate::RegisterAttributes()
{
    attributesUpdater_->Register( "Mounted", boost::bind( &FOM_Serializer_ABC::ReadMounted, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( mounted_ ) ), Wrapper< double >( mounted_ ) );
    attributesUpdater_->Register( "Echelon", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ), Wrapper< unsigned char >( 14 ) ); // platoon
    attributesUpdater_->Register( "UniqueID", boost::bind( &FOM_Serializer_ABC::ReadUniqueId, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( uniqueId_ ) ), uniqueId_, fomSerializer_.GetUniqueIdSerializer() );
    attributesUpdater_->Register( "HigherHeadquarters", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ), uniqueId_, fomSerializer_.GetUniqueIdSerializer() );
    attributesUpdater_->Register( "Callsign", boost::bind( &FOM_Serializer_ABC::ReadCallsign, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( callsign_ ) ), callsign_ );
    attributesUpdater_->Register( "Status", boost::bind( &FOM_Serializer_ABC::ReadStatus, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( status_ ) ), Wrapper< int8_t >( status_ ) );
    attributesUpdater_->Register( "Symbol", boost::bind( &FOM_Serializer_ABC::ReadSymbol, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( symbol_ ) ), symbol_ );
    attributesUpdater_->Register( "EmbeddedUnitList", boost::bind( &FOM_Serializer_ABC::ReadEmbeddedUnitList, boost::ref( fomSerializer_ ), _1, _2, _3 ), 
                                    VariableArray< std::vector< char > >( ) , fomSerializer_.GetUniqueIdSerializer() );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& NetnAggregate::GetIdentifier( ) const
{
    return aggregate_->GetIdentifier();
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::GetIdentifier
// Created: AHC 2012-04-20
// -----------------------------------------------------------------------------
void NetnAggregate::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::EmbarkmentChanged
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void NetnAggregate::PlatformAdded( const std::string& /*name*/, unsigned int /*id*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void NetnAggregate::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void NetnAggregate::ParentChanged( const std::string& /*parentId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void NetnAggregate::StateChanged( rpr::DamageState32 /*state*/ )
{
    // NOTHING
}

Agent_ABC* const NetnAggregate::GetAgent() const
{
    return aggregate_->GetAgent();
}
