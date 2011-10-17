// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Aircraft.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "AttributesSerializer.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Aircraft constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
Aircraft::Aircraft( Agent_ABC& agent, unsigned int identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
    : agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "EntityType", type );
    attributes_->Register( "EntityIdentifier", rpr::EntityIdentifier( 1, 1, static_cast< unsigned short >( identifier ) ) );
    attributes_->Register( "ForceIdentifier", Wrapper< unsigned char >( static_cast< unsigned char >( force ) ) );
    attributes_->Register( "Marking", Marking( name ) );
    attributes_->Register( "Spatial", Spatial( true, 0., 0., 0., 0., 0. ) );
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Aircraft destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
Aircraft::~Aircraft()
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Aircraft::Serialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: Aircraft::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Aircraft::SpatialChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    attributes_->Update( "Spatial", Spatial( true, latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: Aircraft::FormationChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Aircraft::EquipmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Aircraft::EmbarkmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void Aircraft::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}
