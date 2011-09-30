// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RemoteAggregate.h"
#include "hla_plugin/Spatial.h"
#include "hla_plugin/AggregateMarking.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/SilentEntity.h"
#include "rpr/EntityType.h"
#include "MockUpdateFunctor.h"
#include "MockRemoteAgentListener.h"
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <vector>

using namespace plugins::hla;

namespace
{
    typedef std::vector< int8 > T_Buffer;
    class Fixture
    {
    public:
        Fixture()
            : aggregate( "identifier", listener )
        {}
        ::hla::Deserializer Deserialize()
        {
            buffer.resize( serializer.GetSize() );
            serializer.CopyTo( &buffer[0] );
            return ::hla::Deserializer( &buffer[0], buffer.size() );
        }
        MockRemoteAgentListener listener;
        RemoteAggregate aggregate;
        ::hla::Serializer serializer;
        T_Buffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_cannot_be_serialized, Fixture )
{
    ::hla::MockUpdateFunctor functor;
    BOOST_CHECK_THROW( aggregate.Serialize( functor, true ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_spatial_attribute_and_notifies_listener, Fixture )
{
    Spatial spatial( true, 1., 2., 3., 4., 5. );
    spatial.Serialize( serializer );
    MOCK_EXPECT( listener, Moved ).once().with( "identifier", mock::close( 1., 0.001 ), mock::close( 2., 0.001 ) );
    aggregate.Deserialize( "Spatial", Deserialize() );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_force_identifier_attribute_and_notifies_listener, Fixture )
{
    serializer << static_cast< int8 >( rpr::Friendly );
    MOCK_EXPECT( listener, SideChanged ).once().with( "identifier", rpr::Friendly );
    aggregate.Deserialize( "ForceIdentifier", Deserialize() );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_aggregate_marking_attribute_and_notifies_listener, Fixture )
{
    const AggregateMarking marking( "name" );
    marking.Serialize( serializer );
    MOCK_EXPECT( listener, NameChanged ).once().with( "identifier", "name" );
    aggregate.Deserialize( "AggregateMarking", Deserialize() );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_entity_type_attribute_and_notifies_listener, Fixture )
{
    const rpr::EntityType type( "1 2 3" );
    type.Serialize( serializer );
    MOCK_EXPECT( listener, TypeChanged ).once().with( "identifier", rpr::EntityType( "1 2 3 0 0 0 0" ) );
    aggregate.Deserialize( "EntityType", Deserialize() );
}

BOOST_FIXTURE_TEST_CASE( remote_aggregate_deserializes_silent_entities_attribute_and_notifies_listener_for_each_entity, Fixture )
{
    const uint32 numberOfSilentEntities = 2;
    serializer << numberOfSilentEntities;
    aggregate.Deserialize( "NumberOfSilentEntities", Deserialize() );
    serializer = ::hla::Serializer();
    const SilentEntity firstSilentEntity( rpr::EntityType( "1 2 3" ), 42 );
    const SilentEntity secondSilentEntity( rpr::EntityType( "4 5 6" ), 43 );
    firstSilentEntity.Serialize( serializer );
    secondSilentEntity.Serialize( serializer );
    mock::sequence s;
    MOCK_EXPECT( listener, EquipmentUpdated ).once().in( s ).with( "identifier", rpr::EntityType( "1 2 3" ), 42u );
    MOCK_EXPECT( listener, EquipmentUpdated ).once().in( s ).with( "identifier", rpr::EntityType( "4 5 6" ), 43u );
    aggregate.Deserialize( "SilentEntities", Deserialize() );
}
