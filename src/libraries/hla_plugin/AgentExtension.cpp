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
#include "clients_kernel/Karma.h"
#include "dispatcher/Equipment.h"
#include "protocol/Protocol.h"
#include "rpr/EntityType.h"
#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Observable< sword::UnitAttributes >& attributes,
                                dispatcher::Observable< sword::UnitEnvironmentType >& environment,
                                Agent_ABC& holder, const rpr::EntityIdentifier& id )
    : Observer< sword::UnitAttributes >( attributes )
    , Observer< sword::UnitEnvironmentType >( environment )
    , holder_            ( holder )
    , id_                ( id )
    , spatialChanged_    ( true )
    , compositionChanged_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AgentExtension::Serialize( ::hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    if( bUpdateAll )
        UpdateEntityType( functor );
    if( bUpdateAll )
        UpdateEntityIdentifier( functor );
    if( bUpdateAll || spatialChanged_ )
        UpdateSpatial( functor );
    if( bUpdateAll )
        UpdateAggregateMarking( functor );
    if( bUpdateAll )
        UpdateAggregateState( functor );
    if( bUpdateAll )
        UpdateForceIdentifier( functor );
    formation_.Serialize( functor, bUpdateAll );
    if( bUpdateAll || compositionChanged_ )
        UpdateComposition( functor );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Notify
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
void AgentExtension::Notify( const sword::UnitAttributes& attributes )
{
    spatialChanged_ = spatialChanged_ || attributes.has_position()
                                      || attributes.has_height()
                                      || attributes.has_speed()
                                      || attributes.has_direction();
    compositionChanged_ = compositionChanged_ || attributes.has_equipment_dotations();
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Notify
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::Notify( const sword::UnitEnvironmentType& attributes )
{
    formation_.Update( attributes );
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
    Spatial spatial( holder_.GetPosition().X(), holder_.GetPosition().Y(), static_cast< float >( holder_.GetAltitude() ), static_cast< float >( holder_.GetSpeed() ), static_cast< float >( holder_.GetDirection() ) );
    ::hla::Serializer archive;
    spatial.Serialize( archive );
    functor.Visit( ::hla::AttributeIdentifier( "Spatial" ), archive );
    spatialChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateAggregateMarking
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateAggregateMarking( ::hla::UpdateFunctor_ABC& functor ) const
{
    AggregateMarking marking( holder_.GetName() );
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
    unsigned char force = 0; // Other
    const kernel::Karma& karma = holder_.GetForce();
    if( karma == kernel::Karma::friend_ )
        force = 1;
    else if( karma == kernel::Karma::enemy_ )
        force = 2;
    else if( karma == kernel::Karma::neutral_ )
        force = 3;
    ::hla::Serializer archive;
    archive << force;
    functor.Visit( ::hla::AttributeIdentifier( "ForceIdentifier" ), archive );
}

namespace
{
    struct SilentEntitiesSerializer
    {
        SilentEntitiesSerializer()
            : count_( 0 )
        {
            // NOTHING
        }
        void SerializeEquipment( const dispatcher::Equipment& e )
        {
            ++count_;
            rpr::EntityType type( "1 1 225 1" );
            SilentEntity entity( type, static_cast< unsigned short >( e.nNbrAvailable_ ) );
            entity.Serialize( serializer_ );
        }
        void Commit( ::hla::UpdateFunctor_ABC& functor )
        {
            {
                ::hla::Serializer archive;
                archive << count_;
                functor.Visit( ::hla::AttributeIdentifier( "NumberOfSilentEntities" ), archive );
            }
            functor.Visit( ::hla::AttributeIdentifier( "SilentEntities" ), serializer_ );
        }
        unsigned short count_;
        ::hla::Serializer serializer_;
    };
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateComposition
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void AgentExtension::UpdateComposition( ::hla::UpdateFunctor_ABC& functor ) const
{
    SilentEntitiesSerializer serializer;
    holder_.GetEquipments().Apply( boost::bind( &SilentEntitiesSerializer::SerializeEquipment, &serializer, _1 ) );
    serializer.Commit( functor );
}
