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
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "Formation.h"
#include "Dimension.h"
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
    : agent_             ( agent )
    , identifier_        ( identifier )
    , name_              ( name )
    , force_             ( force )
    , type_              ( type )
    , formationChanged_  ( false )
    , dimensionsChanged_ ( false )
    , isOnRoad_          ( false )
    , spatialChanged_    ( false )
    , pSpatial_          ( 0 )
    , compositionChanged_( false )
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

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AggregateEntity::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    if( updateAll )
        UpdateEntityType( functor );
    if( updateAll )
        UpdateEntityIdentifier( functor );
    if( updateAll || spatialChanged_ )
        UpdateSpatial( functor );
    if( updateAll )
        UpdateAggregateMarking( functor );
    if( updateAll )
        UpdateAggregateState( functor );
    if( updateAll || dimensionsChanged_ )
        UpdateDimensions( functor );
    if( updateAll )
        UpdateForceIdentifier( functor );
    if( updateAll || formationChanged_ )
        UpdateFormation( functor );
    if( updateAll || compositionChanged_ )
        UpdateComposition( functor );
    if( updateAll )
        UpdateEchelon( functor );
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
    compositionChanged_ = true;
    IT_Equipments result = std::find_if( equipments_.begin(), equipments_.end(), boost::bind( &::Find, type, _1 ) );
    if( result == equipments_.end() )
        equipments_.push_back( std::make_pair( type, available ) );
    else
        result->second = available;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::SpatialChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    spatialChanged_ = true;
    pSpatial_.reset( new Spatial( latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::FormationChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::FormationChanged( bool isOnRoad )
{
    isOnRoad_ = isOnRoad;
    formationChanged_ = true;
    dimensionsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateFormation
// Created: SLI 2011-06-08
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateFormation( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer serializer;
    Formation formation( isOnRoad_ );
    formation.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "Formation" ), serializer );
    formationChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateDimensions
// Created: SLI 2011-06-08
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateDimensions( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer serializer;
    Dimension dimensions( isOnRoad_ );
    dimensions.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "Dimensions" ), serializer );
    dimensionsChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateEntityType
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateEntityType( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer serializer;
    type_.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "EntityType" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateEntityIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateEntityIdentifier( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer serializer;
    identifier_.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "EntityIdentifier" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateSpatial
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateSpatial( ::hla::UpdateFunctor_ABC& functor ) const
{
    if( !pSpatial_.get() )
        return;
    ::hla::Serializer archive;
    pSpatial_->Serialize( archive );
    functor.Visit( ::hla::AttributeIdentifier( "Spatial" ), archive );
    spatialChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateAggregateMarking
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateAggregateMarking( ::hla::UpdateFunctor_ABC& functor ) const
{
    AggregateMarking marking( name_ );
    ::hla::Serializer archive;
    marking.Serialize( archive );
    functor.Visit( ::hla::AttributeIdentifier( "AggregateMarking" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateAggregateState
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateAggregateState( ::hla::UpdateFunctor_ABC& functor ) const
{
    unsigned char state = 1; // fully aggregated
    ::hla::Serializer archive;
    archive << state;
    functor.Visit( ::hla::AttributeIdentifier( "AggregateState" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateForceIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateForceIdentifier( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer archive;
    archive << static_cast< unsigned char >( force_ );
    functor.Visit( ::hla::AttributeIdentifier( "ForceIdentifier" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateComposition
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateComposition( ::hla::UpdateFunctor_ABC& functor ) const
{
    {
        ::hla::Serializer serializer;
        serializer << static_cast< unsigned short >( equipments_.size() );
        functor.Visit( ::hla::AttributeIdentifier( "NumberOfSilentEntities" ), serializer );
    }
    ::hla::Serializer serializer;
    BOOST_FOREACH( const T_Equipment& equipment, equipments_ )
    {
        const rpr::EntityType type( "1 1 225 1" );
        const SilentEntity entity( type, static_cast< unsigned short >( equipment.second ) );
        entity.Serialize( serializer );
    }
    functor.Visit( ::hla::AttributeIdentifier( "SilentEntities" ), serializer );
    compositionChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::UpdateEchelon
// Created: SLI 2011-06-16
// -----------------------------------------------------------------------------
void AggregateEntity::UpdateEchelon( ::hla::UpdateFunctor_ABC& functor ) const
{
    unsigned char state = 14; // platoon
    ::hla::Serializer archive;
    archive << state;
    functor.Visit( ::hla::AttributeIdentifier( "Echelon" ), archive );
}
