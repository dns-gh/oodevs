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
#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateEntity constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AggregateEntity::AggregateEntity( Agent_ABC& agent, const rpr::EntityIdentifier& identifier,
                                  const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
    : agent_                 ( agent )
    , identifier_            ( identifier )
    , name_                  ( name )
    , force_                 ( force )
    , type_                  ( type )
    , spatial_               ( Spatial( 0., 0., 0., 0., 0. ) )
    , formation_             ( Formation( false ) )
    , dimensions_            ( Dimension( false ) )
    , numberOfSilentEntities_( 0 )
    , mounted_               ( 0. )
{
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

namespace
{
    template< typename T >
    ::hla::Serializer& operator<<( ::hla::Serializer& serializer, const T& value )
    {
        value.Serialize( serializer );
        return serializer;
    }
    template< typename T >
    void Serialize( const std::string& attribute, const T& value, ::hla::UpdateFunctor_ABC& functor, bool updateAll )
    {
        if( updateAll )
        {
            ::hla::Serializer serializer;
            serializer << value;
            functor.Visit( ::hla::AttributeIdentifier( attribute ), serializer );
        }
    }
    template< typename T >
    void Serialize( const std::string& attribute, const Dynamic< T >& value, ::hla::UpdateFunctor_ABC& functor, bool updateAll )
    {
        if( updateAll || value.HasChanged() )
        {
            ::hla::Serializer serializer;
            serializer << value;
            functor.Visit( ::hla::AttributeIdentifier( attribute ), serializer );
        }
    }
    template< typename T >
    void Serialize( const std::string& attribute, const DynamicArray< T >& value, ::hla::UpdateFunctor_ABC& functor, bool updateAll )
    {
        if( updateAll || value.HasChanged() )
        {
            ::hla::Serializer serializer;
            serializer << value;
            functor.Visit( ::hla::AttributeIdentifier( attribute ), serializer );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AggregateEntity::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    ::Serialize( "EntityType", type_, functor, updateAll );
    ::Serialize( "EntityIdentifier", identifier_, functor, updateAll );
    ::Serialize( "Spatial", spatial_, functor, updateAll );
    ::Serialize( "AggregateMarking", AggregateMarking( name_ ), functor, updateAll );
    ::Serialize( "AggregateState", static_cast< unsigned char >( 1 ), functor, updateAll ); // fully aggregated
    ::Serialize( "Dimensions", dimensions_, functor, updateAll );
    ::Serialize( "ForceIdentifier", static_cast< unsigned char >( force_ ), functor, updateAll );
    ::Serialize( "Formation", formation_, functor, updateAll );
    ::Serialize( "NumberOfSilentEntities", numberOfSilentEntities_, functor, updateAll );
    ::Serialize( "SilentEntities", silentEntities_, functor, updateAll );
    ::Serialize( "Mounted", mounted_, functor, updateAll );
    ::Serialize( "Echelon", static_cast< double >( 14. ), functor, updateAll ); // platoon
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Deserialize
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void AggregateEntity::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, const ::hla::Deserializer& /*deserializer*/ )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

namespace
{
    bool Find( unsigned int type, const std::pair< unsigned int, unsigned int >& value )
    {
        return value.first == type;
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::EquipmentChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::EquipmentChanged( unsigned int type, unsigned int available )
{
    IT_Equipments result = std::find_if( equipments_.begin(), equipments_.end(), boost::bind( &::Find, type, _1 ) );
    if( result == equipments_.end() )
        equipments_.push_back( std::make_pair( type, available ) );
    else
        result->second = available;
    numberOfSilentEntities_.Update( static_cast< unsigned short >( equipments_.size() ) );
    std::vector< SilentEntity > entities;
    BOOST_FOREACH( const T_Equipment& equipment, equipments_ )
        entities.push_back( SilentEntity( rpr::EntityType ( "1 1 225 1" ), static_cast< unsigned short >( equipment.second ) ) ); // $$$$ _RC_ SLI 2011-06-16: resolve platform types
    silentEntities_.Update( entities );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::SpatialChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    spatial_.Update( Spatial( latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::FormationChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::FormationChanged( bool isOnRoad )
{
    formation_.Update( Formation( isOnRoad ) );
    dimensions_.Update( Dimension( isOnRoad ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::EmbarkmentChanged
// Created: SLI 2011-06-16
// -----------------------------------------------------------------------------
void AggregateEntity::EmbarkmentChanged( bool mounted )
{
    mounted_.Update( mounted ? 100. : 0. );
}
