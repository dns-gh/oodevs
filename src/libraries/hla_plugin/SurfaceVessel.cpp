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
#include "MarkingFactory_ABC.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SurfaceVessel constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::SurfaceVessel( Agent_ABC& agent, unsigned int identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              unsigned short siteID, unsigned short applicationID )
    : identifier_( name )
    , listeners_ ( new ObjectListenerComposite() )
    , agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "EntityType", type );
    attributes_->Register( "EntityIdentifier", rpr::EntityIdentifier( siteID, applicationID, static_cast< unsigned short >( identifier ) ) );
    attributes_->Register( "ForceIdentifier", Wrapper< unsigned char >( static_cast< unsigned char >( force ) ) );
    attributes_->Register( "Marking", markingFactory.CreateMarking( name, identifier ) );
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
void SurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer_ABC& /*deserializer*/ )
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

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::SetIdentifier
// Created: AHC 2012-03-15
// -----------------------------------------------------------------------------
void SurfaceVessel::SetIdentifier( const std::string& id )
{
    identifier_ = id;
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& SurfaceVessel::GetIdentifier( ) const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void SurfaceVessel::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void SurfaceVessel::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}
