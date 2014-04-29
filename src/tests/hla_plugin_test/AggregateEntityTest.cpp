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
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "DeserializationTools.h"
#include "MockAgent.h"
#include "MockUpdateFunctor.h"
#include "MockMarkingFactory.h"
#include "MockFOM_Serialization.h"
#include "MockEntityIdentifierResolver.h"
#include <hla/Deserializer.h>
#include <hla/Serializer.h>
#include <hla/AttributeIdentifier.h>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : listener( 0 )
            , entityId( 42, 43, 1 )
        {
            MOCK_EXPECT( agent.Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( agent.Unregister ).once();
            MOCK_EXPECT( factory.CreateAggregateMarking ).once().returns( AggregateMarking() );
        }
        MockAgent agent;
        MockMarkingFactory factory;
        MockEntityIdentifierResolver entityIdResolver;
        EventListener_ABC* listener;
        hla::MockUpdateFunctor functor;
        rpr::EntityType entityType;
        MockFOM_Serialization fomSerialization;
        rpr::EntityIdentifier entityId;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : entity( agent, "name", rpr::Friendly, rpr::EntityType(), factory, entityId, entityIdResolver, fomSerialization, "rtiId", 42 )
        {}
        AggregateEntity entity;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_cannot_be_deserialized, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_NO_THROW( entity.Deserialize( "identifier", deserializer ) ); //, std::exception );
}

BOOST_FIXTURE_TEST_CASE( unmodified_agent_serializes_nothing, RegisteredFixture )
{
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( agregate_entity_serializes_all_its_attributes, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "AggregateMarking" )
                                                                        ( "AggregateState" )
                                                                        ( "Dimensions" )
                                                                        ( "Spatial" )
                                                                        ( "Formation" )
                                                                        ( "NumberOfSilentEntities" )
                                                                        ( "SilentEntities" )
                                                                        ( "SubAggregateIdentifiers" )
                                                                        ( "EntityIdentifiers" )
                                                                        ( "NumberOfVariableDatums" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
}

namespace rpr
{
    template <typename Archive>
    void operator >> (  Archive& archive, EntityIdentifier& id )
    {
        id.Deserialize( archive );
    }
    std::ostream& operator << ( std::ostream& os, const EntityIdentifier& id )
    {
        return os << id.str();
    }
}

namespace
{
    template< typename T >
    bool CheckSerialization( ::hla::T_SerializerPtr serializer, const T& expected )
    {
        std::vector< uint8_t > buffer( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        T actual;
        deserializer >> actual;
        BOOST_CHECK_EQUAL( expected, actual );
        BOOST_CHECK_THROW( deserializer >> actual, std::exception );
        return true;
    }
    bool CheckSize( ::hla::T_SerializerPtr serializer, unsigned int size )
    {
        BOOST_CHECK_EQUAL( size, serializer->GetSize() );
        return true;
    }
    const unsigned int SILENT_ENTITY_SIZE = 2 * sizeof( int16_t ) + 6 * sizeof( int8_t ) + 1 * sizeof( int16_t );
}

BOOST_FIXTURE_TEST_CASE( agent_has_entity_identifier, RegisteredFixture )
{
    const rpr::EntityIdentifier expectedId( 42, 43, 1);
    MOCK_EXPECT( functor.Visit ).once().with( "EntityIdentifier", boost::bind( &CheckSerialization< rpr::EntityIdentifier >, _1, expectedId ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_is_fully_aggregated, RegisteredFixture )
{
    const unsigned char fullyAggregated = 1;
    MOCK_EXPECT( functor.Visit ).once().with( "AggregateState", boost::bind( &CheckSerialization< unsigned char >, _1, fullyAggregated ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_has_no_sub_aggregate_identifiers, RegisteredFixture )
{
    const uint32_t numberSubAggregateIdentifiers = 0u;
    MOCK_EXPECT( functor.Visit ).once().with( "SubAggregateIdentifiers", boost::bind( &CheckSerialization< uint32_t >, _1, numberSubAggregateIdentifiers ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_has_no_entity_identifiers, RegisteredFixture )
{
    const uint32_t numberOfEntityIdentifiers = 0u;
    MOCK_EXPECT( functor.Visit ).once().with( "EntityIdentifiers", boost::bind( &CheckSerialization< uint32_t >, _1, numberOfEntityIdentifiers ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( empty_agent_does_not_have_any_silent_entity, RegisteredFixture )
{
    const unsigned short numberOfSilentEntity = 0;
    MOCK_EXPECT( functor.Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor.Visit ).once().with( "SilentEntities", boost::bind( &CheckSize, _1, 0 * SILENT_ENTITY_SIZE ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_updates_already_known_equipment, RegisteredFixture )
{
    const unsigned short numberOfSilentEntity = 1;
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, entityType, 1u, 0u, 0u, 0u );
    listener->EquipmentChanged( 1u, entityType, 2u, 0u, 0u, 0u );
    MOCK_EXPECT( functor.Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor.Visit ).once().with( "SilentEntities", boost::bind( &CheckSize, _1, 1 * SILENT_ENTITY_SIZE ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_serializes_silent_entities_number, RegisteredFixture )
{
    const unsigned short numberOfSilentEntity = 2;
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, entityType, 1u, 0u, 0u, 0u );
    listener->EquipmentChanged( 2u, entityType, 2u, 0u, 0u, 0u );
    MOCK_EXPECT( functor.Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor.Visit ).once().with( "SilentEntities", boost::bind( &CheckSize, _1, 2 * SILENT_ENTITY_SIZE ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( spatial_changed_event_is_serialized, RegisteredFixture )
{
    BOOST_REQUIRE( listener );
    listener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( functor.Visit ).once().with( "Spatial", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( formation_changed_event_is_serialized, RegisteredFixture )
{
    BOOST_REQUIRE( listener );
    listener->FormationChanged( true );
    mock::sequence s;
    MOCK_EXPECT( functor.Visit ).once().in( s ).with( "Dimensions", mock::any );
    MOCK_EXPECT( functor.Visit ).once().in( s ).with( "Formation", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( equipment_changed_event_is_serialized, RegisteredFixture )
{
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, entityType, 2u, 0u, 0u, 0u );
    mock::sequence s;
    MOCK_EXPECT( functor.Visit ).once().in( s ).with( "NumberOfSilentEntities", mock::any );
    MOCK_EXPECT( functor.Visit ).once().in( s ).with( "SilentEntities", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( spatial_uses_drm_fpw_when_updated, RegisteredFixture )
{
    plugins::hla::Spatial recvSpatial;
    BOOST_REQUIRE( listener );
    listener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( functor.Visit ).once().with( "Spatial", boost::bind( &GetSerializedValue<plugins::hla::Spatial>, _1, boost::ref(recvSpatial) ) );
    entity.Serialize( functor, false );
    BOOST_CHECK_EQUAL( recvSpatial.isStatic_, false );
    BOOST_CHECK_EQUAL( recvSpatial.deadReckoningAlgorithm_, 2 );
}
