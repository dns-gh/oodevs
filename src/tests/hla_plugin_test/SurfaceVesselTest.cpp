// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SurfaceVessel.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "MockAgent.h"
#include "MockUpdateFunctor.h"
#include "MockMarkingFactory.h"
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
            MOCK_EXPECT( factory, CreateMarking ).once().returns( Marking() );
        }
        MockAgent agent;
        MockMarkingFactory factory;
        EventListener_ABC* listener;
        hla::MockUpdateFunctor functor;
        rpr::EntityType entityType;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : entity( agent, 1u, "name", rpr::Friendly, rpr::EntityType(), factory )
        {}
        SurfaceVessel entity;
    };
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_cannot_be_deserialized, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_THROW( entity.Deserialize( "identifier", deserializer ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( unmodified_surface_vessel_serializes_nothing, RegisteredFixture )
{
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_serializes_all_its_attributes, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "Marking" )
                                                                        ( "Spatial" );
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
    bool CheckSize( const ::hla::Serializer& serializer, unsigned int size )
    {
        BOOST_CHECK_EQUAL( size, serializer.GetSize() );
        return true;
    }
    const unsigned int SILENT_ENTITY_SIZE = 2 * sizeof( int16 ) + 6 * sizeof( int8 ) + 1 * sizeof( int16 ) + sizeof( int32 );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_spatial_changed_event_is_serialized, RegisteredFixture )
{
    BOOST_REQUIRE( listener );
    listener->SpatialChanged( 1., 2., 3., 4., 5. );
    MOCK_EXPECT( functor, Visit ).once().with( "Spatial", mock::any );
    entity.Serialize( functor, false );
    entity.Serialize( functor, false );
}
