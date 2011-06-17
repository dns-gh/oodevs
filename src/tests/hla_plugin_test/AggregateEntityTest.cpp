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
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "MockAgent.h"
#include "MockUpdateFunctor.h"
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
        {
            MOCK_EXPECT( agent, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( agent, Unregister ).once();
        }
        MockAgent agent;
        EventListener_ABC* listener;
        hla::MockUpdateFunctor functor;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_cannot_be_deserialized, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_THROW( entity.Deserialize( "identifier", deserializer ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( unmodified_agent_serializes_nothing, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( agent_serializes_all, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "Spatial" )
                                                                        ( "AggregateMarking" )
                                                                        ( "AggregateState" )
                                                                        ( "Dimensions" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "Formation" )
                                                                        ( "NumberOfSilentEntities" )
                                                                        ( "SilentEntities" )
                                                                        ( "Mounted" )
                                                                        ( "Echelon" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor, Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor, Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
}

namespace
{
    template< typename T >
    bool CheckSerialization( const ::hla::Serializer& serializer, const T& expected )
    {
        std::vector< int8 > buffer( serializer.GetSize() );
        if( !buffer.empty() )
            serializer.CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        T actual;
        deserializer >> actual;
        BOOST_CHECK_EQUAL( expected, actual );
        BOOST_CHECK_THROW( deserializer >> actual, std::runtime_error );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( agent_is_fully_aggregated, Fixture )
{
    const unsigned char fullyAggregated = 1;
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    MOCK_EXPECT( functor, Visit ).once().with( "AggregateState", boost::bind( &CheckSerialization< unsigned char >, _1, fullyAggregated ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_echelon_is_platoon, Fixture )
{
    const double platoonEchelon = 14;
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    hla::MockUpdateFunctor functor;
    MOCK_EXPECT( functor, Visit ).once().with( "Echelon", boost::bind( &CheckSerialization< double >, _1, platoonEchelon ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( empty_agent_does_not_have_any_silent_entity, Fixture )
{
    const unsigned short numberOfSilentEntity = 0;
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    MOCK_EXPECT( functor, Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_serializes_silent_entities_number, Fixture )
{
    const unsigned short numberOfSilentEntity = 2;
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, 1u );
    listener->EquipmentChanged( 2u, 2u );
    MOCK_EXPECT( functor, Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_updates_already_known_equipment, Fixture )
{
    const unsigned short numberOfSilentEntity = 1;
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, 1u );
    listener->EquipmentChanged( 1u, 2u );
    MOCK_EXPECT( functor, Visit ).once().with( "NumberOfSilentEntities", boost::bind( &CheckSerialization< unsigned short >, _1, numberOfSilentEntity ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( spatial_changed_event_is_serialized, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( functor, Visit ).once().with( "Spatial", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( formation_changed_event_is_serialized, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->FormationChanged( true );
    mock::sequence s;
    MOCK_EXPECT( functor, Visit ).once().in( s ).with( "Dimensions", mock::any );
    MOCK_EXPECT( functor, Visit ).once().in( s ).with( "Formation", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( equipment_changed_event_is_serialized, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->EquipmentChanged( 1u, 2u );
    mock::sequence s;
    MOCK_EXPECT( functor, Visit ).once().in( s ).with( "NumberOfSilentEntities", mock::any );
    MOCK_EXPECT( functor, Visit ).once().in( s ).with( "SilentEntities", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( embarkment_changed_event_is_serialized, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    BOOST_REQUIRE( listener );
    listener->EmbarkmentChanged( true );
    MOCK_EXPECT( functor, Visit ).once().with( "Mounted", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}
