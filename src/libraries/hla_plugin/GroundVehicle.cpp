// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "GroundVehicle.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "AttributesSerializer.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include "MarkingFactory_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: GroundVehicle constructor
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
GroundVehicle::GroundVehicle( Agent_ABC& agent, unsigned int identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              unsigned short siteID, unsigned short applicationID, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& fomSerializer )
    :  identifier_( name )
    , listeners_ ( new ObjectListenerComposite() )
    , fomSerializer_( fomSerializer )
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
// Name: GroundVehicle destructor
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
GroundVehicle::~GroundVehicle( )
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::Serialize
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::Deserialize
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer_ABC& /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::SpatialChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    attributes_->Update( "Spatial", Spatial( false, latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::FormationChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::EquipmentChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::EmbarkmentChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::SetIdentifier
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::SetIdentifier( const std::string& id )
{
    identifier_ = id;
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::GetIdentifier
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
const std::string& GroundVehicle::GetIdentifier( ) const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::Register
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::Unregister
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void GroundVehicle::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::EmbarkmentChanged
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void GroundVehicle::PlatformAdded( const std::string& /*name*/, unsigned int /*id*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::EmbarkmentChanged
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void GroundVehicle::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::EmbarkmentChanged
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void GroundVehicle::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void GroundVehicle::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroundVehicle::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void GroundVehicle::ParentChanged( const std::string& /*parentId*/ )
{
    // NOTHING
}