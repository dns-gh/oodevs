// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnSurfaceVessel.h"
#include "MockHlaObject.h"
#include "MockUpdateFunctor.h"
#include "MockAgent.h"
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
            : listener ( 0 )
            , aggregate( new MockHlaObject() )
        {}
        MockHlaObject* aggregate;
        MockAgent agent;
        EventListener_ABC* listener;
        hla::MockUpdateFunctor functor;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : entity( std::auto_ptr< HlaObject_ABC >( aggregate ), agent, "name", "identifier", "symbol" )
        {}
        NetnSurfaceVessel entity;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_surface_vessel_cannot_be_deserialized, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_THROW( entity.Deserialize( "identifier", deserializer ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( unmodified_netn_surface_vessel_serializes_nothing, RegisteredFixture )
{
    MOCK_EXPECT( aggregate, Serialize ).once();
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( netn_surface_vessel_serializes_all_its_attributes_but_rpr_aggregate_first, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "UniqueID" )
                                                                        ( "Callsign" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        MOCK_EXPECT( aggregate, Serialize ).once().in( s );
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor, Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        MOCK_EXPECT( aggregate, Serialize ).once().in( s );
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
    class SurfaceVesselFixture : public RegisteredFixture
    {
    public:
        SurfaceVesselFixture()
        {
            MOCK_EXPECT( aggregate, Serialize ).once();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_unique_id_is_sword_plus_identifier, SurfaceVesselFixture )
{
    MOCK_EXPECT( functor, Visit ).once().with( "UniqueID", boost::bind( &CheckSize, _1, 11u ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_callsign_is_its_name, SurfaceVesselFixture )
{
    MOCK_EXPECT( functor, Visit ).once().with( "Callsign", boost::bind( &CheckSize, _1, sizeof( uint32 ) + 4 * sizeof( uint16 ) ) );
    MOCK_EXPECT( functor, Visit );
    entity.Serialize( functor, true );
}
