// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateEntity.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "Formation.h"
#include "Dimension.h"
#include "AttributesSerializer.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateEntity constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AggregateEntity::AggregateEntity( Agent_ABC& agent, unsigned int identifier,
                                  const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
    : agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "EntityType", type );
    attributes_->Register( "EntityIdentifier", rpr::EntityIdentifier( 1, 1, static_cast< unsigned short >( identifier ) ) );
    attributes_->Register( "ForceIdentifier", Wrapper< unsigned char >( static_cast< unsigned char >( force ) ) );
    attributes_->Register( "AggregateMarking", AggregateMarking( name ) );
    attributes_->Register( "AggregateState", Wrapper< unsigned char >( 1 ) ); // fully aggregated
    attributes_->Register( "Dimensions", Dimension( false ) );
    attributes_->Register( "Spatial", Spatial( true, 0., 0., 0., 0., 0. ) );
    attributes_->Register( "Formation", Formation( false ) );
    attributes_->Register( "NumberOfSilentEntities", Wrapper< unsigned short >( 0 ) );
    attributes_->Register( "SilentEntities", Wrapper< std::vector< SilentEntity > >( std::vector< SilentEntity >() ) );
    attributes_->Register( "SilentAggregates", Wrapper< uint32 >( 0 ) ); // no aggregates
    attributes_->Register( "SubAggregateIdentifiers", Wrapper< uint32 >( 0 ) ); // no sub aggregates identifiers
    attributes_->Register( "EntityIdentifiers", Wrapper< uint32 >( 0 ) ); // no entity identifiers
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AggregateEntity::~AggregateEntity()
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AggregateEntity::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Deserialize
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void AggregateEntity::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

namespace
{
    template< typename T >
    bool Find( unsigned int type, const T& value )
    {
        return value.type_ == type;
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::EquipmentChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available )
{
    IT_Equipments result = std::find_if( equipments_.begin(), equipments_.end(), boost::bind( &::Find< T_Equipment >, type, _1 ) );
    if( result == equipments_.end() )
        equipments_.push_back( T_Equipment( type, available, entityType ) );
    else
        result->available_ = available;
    attributes_->Update( "NumberOfSilentEntities", Wrapper< unsigned short >( static_cast< unsigned short >( equipments_.size() ) ) );
    std::vector< SilentEntity > entities;
    BOOST_FOREACH( const T_Equipment& equipment, equipments_ )
        entities.push_back( SilentEntity( equipment.entityType_, static_cast< unsigned short >( equipment.available_ ) ) );
    attributes_->Update( "SilentEntities", Wrapper< std::vector< SilentEntity > >( entities ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::SpatialChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    attributes_->Update( "Spatial", Spatial( true, latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::FormationChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::FormationChanged( bool isOnRoad )
{
    attributes_->Update( "Formation", Formation( isOnRoad ) );
    attributes_->Update( "Dimensions", Dimension( isOnRoad ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::EmbarkmentChanged
// Created: SLI 2011-06-16
// -----------------------------------------------------------------------------
void AggregateEntity::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}
