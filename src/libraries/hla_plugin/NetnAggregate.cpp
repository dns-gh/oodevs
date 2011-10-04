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
#include "AttributesSerializer.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregate::NetnAggregate( std::auto_ptr< HlaObject_ABC > aggregate, Agent_ABC& agent, const std::string& name, short identifier )
    : aggregate_ ( aggregate )
    , agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "Mounted", Wrapper< double >( 0. ) ); // 0%
    attributes_->Register( "Echelon", Wrapper< unsigned char >( 14 ) ); // platoon
    attributes_->Register( "UniqueID", UniqueId( "SWORD" + boost::lexical_cast< std::string >( identifier ) ) );
    attributes_->Register( "HigherHeadquarters", UniqueId( "SWORD" + boost::lexical_cast< std::string >( identifier ) ) );
    attributes_->Register( "Callsign", UnicodeString( name ) );
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregate::~NetnAggregate()
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    aggregate_->Serialize( functor, updateAll );
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
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
void NetnAggregate::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregate::EmbarkmentChanged
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnAggregate::EmbarkmentChanged( bool mounted )
{
    attributes_->Update( "Mounted", Wrapper< double >( mounted ? 100. : 0. ) );
}
