// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AggregateEntity.h"
#include "hla_plugin/Spatial.h"
#include "hla_plugin/AggregateMarking.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/SilentEntity.h"
#include "hla_plugin/IsPartOfStruct.h"
#include "rpr/EntityType.h"
#include "MockUpdateFunctor.h"
#include "MockObjectListener.h"
#include "MockEntityIdentifierResolver.h"
#include "hla_plugin/FOM_Serializer.h"
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <vector>

using namespace plugins::hla;

namespace
{
    typedef std::vector< uint8_t > T_Buffer;
    class Fixture
    {
    public:
        Fixture()
            : fomSerialization( 1 )
            , aggregate( "identifier", entityIdResolver, fomSerialization )
        {
            aggregate.Register( listener );
        }
        ::hla::Deserializer Deserialize()
        {
            buffer.resize( serializer.GetSize() );
            serializer.CopyTo( &buffer[0] );
            return ::hla::Deserializer( &buffer[0], buffer.size() );
        }
        MockObjectListener listener;
        MockEntityIdentifierResolver entityIdResolver;
        FOM_Serializer fomSerialization;
        AggregateEntity aggregate;
        ::hla::Serializer serializer;
        T_Buffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_can_be_serialized, Fixture )
{
    ::hla::MockUpdateFunctor functor;
    MOCK_EXPECT( functor.Visit );
    BOOST_CHECK_NO_THROW( aggregate.Serialize( functor, true ) );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_spatial_attribute_and_notifies_listener, Fixture )
{
    Spatial spatial( true, 1., 2., 3., 4., 5. );
    spatial.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
    MOCK_EXPECT( listener.Moved ).once().with( "identifier", mock::near( 1., 0.001 ), mock::near( 2., 0.001 ) );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "Spatial", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_force_identifier_attribute_and_notifies_listener, Fixture )
{
    serializer << static_cast< int8_t >( rpr::Friendly );
    MOCK_EXPECT( listener.SideChanged ).once().with( "identifier", rpr::Friendly );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "ForceIdentifier", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_aggregate_marking_attribute_and_notifies_listener, Fixture )
{
    const AggregateMarking marking( "name", 42 );
    marking.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
    MOCK_EXPECT( listener.NameChanged ).once().with( "identifier", "name42" );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "AggregateMarking", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_entity_type_attribute_and_notifies_listener, Fixture )
{
    const rpr::EntityType type( "1 2 3" );
    type.Serialize( serializer );
    MOCK_EXPECT( listener.TypeChanged ).once().with( "identifier", rpr::EntityType( "1 2 3 0 0 0 0" ) );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "EntityType", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_silent_entities_attribute_and_notifies_listener_for_each_entity, Fixture )
{
    const uint16_t numberOfSilentEntities = 2;
    serializer << numberOfSilentEntities;
    {
        ::hla::Deserializer deserializer( Deserialize() );
        aggregate.Deserialize( "NumberOfSilentEntities", deserializer );
    }
    serializer = ::hla::Serializer();
    const SilentEntity firstSilentEntity( rpr::EntityType( "1 2 3" ), 42 );
    const SilentEntity secondSilentEntity( rpr::EntityType( "4 5 6" ), 43 );
    firstSilentEntity.Serialize( serializer );
    secondSilentEntity.Serialize( serializer );
    mock::sequence s;
    MOCK_EXPECT( listener.EquipmentUpdated ).once().in( s ).with( "identifier", rpr::EntityType( "1 2 3" ), 42u );
    MOCK_EXPECT( listener.EquipmentUpdated ).once().in( s ).with( "identifier", rpr::EntityType( "4 5 6" ), 43u );
    {
        ::hla::Deserializer deserializer( Deserialize() );
        aggregate.Deserialize( "SilentEntities", deserializer );
    }
}

/*
BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_ispartort_attribute_and_notifies_listener, Fixture )
{
    IsPartOfStruct v;
    v.rtiId_ = Omt13String( "an_rti_id" );
    v.Serialize( serializer );
    MOCK_EXPECT( listener.ParentChanged ).once().with( "identifier", "an_rti_id" );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "IsPartOf", deserializer );
}
*/

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_sub_aggregates_attribute_and_notifies_listener, Fixture )
{
    plugins::hla::ObjectListener_ABC::T_EntityIDs ids;
    ids.insert( "id1" ); ids.insert( "id2" );
    Omt13StringArray subAggr;
    BOOST_FOREACH( const plugins::hla::ObjectListener_ABC::T_EntityIDs::value_type v, ids )
        subAggr.Add( v );
    
    subAggr.Serialize( serializer );
    MOCK_EXPECT( listener.SubAgregatesChanged ).once().with( "identifier", ids );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "SubAggregateIdentifiers", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_sub_entities_attribute_and_notifies_listener, Fixture )
{
    plugins::hla::ObjectListener_ABC::T_EntityIDs ids;
    ids.insert( "id1" ); ids.insert( "id2" );
    Omt13StringArray subAggr;
    BOOST_FOREACH( const plugins::hla::ObjectListener_ABC::T_EntityIDs::value_type v, ids )
        subAggr.Add( v );
    
    subAggr.Serialize( serializer );
    MOCK_EXPECT( listener.SubEntitiesChanged ).once().with( "identifier", ids );
    ::hla::Deserializer deserializer( Deserialize() );
    aggregate.Deserialize( "EntityIdentifiers", deserializer );
}
