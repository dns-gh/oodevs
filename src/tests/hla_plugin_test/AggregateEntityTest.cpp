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

BOOST_FIXTURE_TEST_CASE( agent_serializes_all_but_spatial_attribute, Fixture )
{
    AggregateEntity entity( agent, rpr::EntityIdentifier(), "name", rpr::Friendly, rpr::EntityType() );
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "AggregateMarking" )
                                                                        ( "AggregateState" )
                                                                        ( "Dimensions" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "Formation" )
                                                                        ( "NumberOfSilentEntities" )
                                                                        ( "SilentEntities" );
    mock::sequence s;
    BOOST_FOREACH( const std::string& attribute, attributes )
        MOCK_EXPECT( functor, Visit ).once().in( s ).with( attribute, mock::any );
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
