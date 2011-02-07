// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentExtension.h"
#include "Agent_ABC.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "SerializationTools.h"
#include "rpr/EntityType.h"
#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( Agent_ABC& agent, const rpr::EntityIdentifier& id,
                                const std::string& name, rpr::ForceIdentifier force )
    : agent_            ( agent )
    , id_                ( id )
    , name_              ( name )
    , force_             ( force )
    , spatialChanged_    ( true )
    , pSpatial_          ( 0 )
    , compositionChanged_( true )
{
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AgentExtension::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
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
    if( updateAll )
        UpdateForceIdentifier( functor );
    formation_.Serialize( functor, updateAll );
    if( updateAll || compositionChanged_ )
        UpdateComposition( functor );
}

namespace
{
    bool Find( unsigned int type, const std::pair< unsigned int, unsigned int >& value )
    {
        return value.first == type;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::EquipmentChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentExtension::EquipmentChanged( unsigned int type, unsigned int available )
{
    compositionChanged_ = true;
    IT_Equipments result = std::find_if( equipments_.begin(), equipments_.end(), boost::bind( &::Find, type, _1 ) );
    if( result == equipments_.end() )
        equipments_.push_back( std::make_pair( type, available ) );
    else
        result->second = available;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::SpatialChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentExtension::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    spatialChanged_ = true;
    pSpatial_.reset( new Spatial( latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::FormationChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentExtension::FormationChanged( bool isOnRoad )
{
    formation_.Update( isOnRoad );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateEntityType
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateEntityType( ::hla::UpdateFunctor_ABC& functor ) const
{
    rpr::EntityType type( "1 1 225 1" );
    ::hla::Serializer serializer;
    type.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "EntityType" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateEntityIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateEntityIdentifier( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer serializer;
    id_.Serialize( serializer );
    functor.Visit( ::hla::AttributeIdentifier( "EntityIdentifier" ), serializer );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateSpatial
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateSpatial( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer archive;
    pSpatial_->Serialize( archive );
    functor.Visit( ::hla::AttributeIdentifier( "Spatial" ), archive );
    spatialChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateAggregateMarking
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateAggregateMarking( ::hla::UpdateFunctor_ABC& functor ) const
{
    AggregateMarking marking( name_ );
    ::hla::Serializer archive;
    marking.Serialize( archive );
    functor.Visit( ::hla::AttributeIdentifier( "AggregateMarking" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateAggregateState
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateAggregateState( ::hla::UpdateFunctor_ABC& functor ) const
{
    unsigned char state = 1; // fully aggregated
    ::hla::Serializer archive;
    archive << state;
    functor.Visit( ::hla::AttributeIdentifier( "AggregateState" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateForceIdentifier
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateForceIdentifier( ::hla::UpdateFunctor_ABC& functor ) const
{
    ::hla::Serializer archive;
    archive << static_cast< unsigned char >( force_ );
    functor.Visit( ::hla::AttributeIdentifier( "ForceIdentifier" ), archive );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateComposition
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateComposition( ::hla::UpdateFunctor_ABC& functor ) const
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
}
