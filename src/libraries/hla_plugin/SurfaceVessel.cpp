// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SurfaceVessel.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "AttributesSerializer.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SurfaceVessel constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::SurfaceVessel( Agent_ABC& agent, unsigned short identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
    : agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "EntityType", type );
    attributes_->Register( "EntityIdentifier", rpr::EntityIdentifier( 1, 1, identifier ) );
    attributes_->Register( "ForceIdentifier", Wrapper< unsigned char >( static_cast< unsigned char >( force ) ) );
    attributes_->Register( "Marking", Marking( name ) );
    attributes_->Register( "Spatial", Spatial( true, 0., 0., 0., 0., 0. ) );
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::~SurfaceVessel()
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Serialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::SpatialChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    attributes_->Update( "Spatial", Spatial( true, latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::FormationChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::EquipmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::EmbarkmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}
