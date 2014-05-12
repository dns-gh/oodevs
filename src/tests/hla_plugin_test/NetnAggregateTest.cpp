// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnAggregate.h"
#include "MockHlaObject.h"
#include "MockAgent.h"
#include "MockUpdateFunctor.h"
#include "MockFOM_Serialization.h"
#include "hla_plugin/UniqueId.h"
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
            , uniqueIdSerializer( 1 )
        {
            MOCK_EXPECT( agent.Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( fomSerialization.GetUniqueIdSerializer ).returns( uniqueIdSerializer );
            MOCK_EXPECT( agent.Unregister ).once();
        }
        MockAgent agent;
        MockHlaObject* aggregate;
        EventListener_ABC* listener;
        hla::MockUpdateFunctor functor;
        UniqueIdSerializer uniqueIdSerializer;
        MockFOM_Serialization fomSerialization;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : entity( std::unique_ptr< HlaObject_ABC >( aggregate ), agent, "name", MakeUniqueId( "identifier" ), "symbol", fomSerialization, "rtiId" )
        {}
        NetnAggregate entity;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_netn_agent_can_be_deserialized, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    MOCK_EXPECT( aggregate->Deserialize ).once();
    entity.Deserialize( "identifier", deserializer );
}

BOOST_FIXTURE_TEST_CASE( unmodified_netn_agent_serializes_nothing, RegisteredFixture )
{
    MOCK_EXPECT( aggregate->Serialize ).once();
    entity.Serialize( functor, false );
}

BOOST_FIXTURE_TEST_CASE( netn_agregate_entity_serializes_all_its_attributes_but_rpr_aggregate_first, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "Mounted" )
                                                                        ( "Echelon" )
                                                                        ( "UniqueID" )
                                                                        ( "HigherHeadquarters" )
                                                                        ( "Callsign" )
                                                                        ( "Status" )
                                                                        ( "Symbol" )
                                                                        ( "EmbeddedUnitList" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        MOCK_EXPECT( aggregate->Serialize ).once().in( s );
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        MOCK_EXPECT( aggregate->Serialize ).once().in( s );
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        entity.Serialize( functor, true );
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
    class AggregateFixture : public RegisteredFixture
    {
    public:
        AggregateFixture()
        {
            MOCK_EXPECT( aggregate->Serialize ).once();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( agent_unique_id_is_sword_plus_identifier, AggregateFixture )
{
    MOCK_EXPECT( functor.Visit ).once().with( "UniqueID", boost::bind( &CheckSize, _1, 11u ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_serializes_app6_symbol_with_unicode_string, AggregateFixture )
{
    MOCK_EXPECT( functor.Visit ).once().with( "Symbol", boost::bind( &CheckSize, _1, sizeof( int32_t ) + 6 * sizeof( uint16_t ) ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_callsign_is_its_name, AggregateFixture )
{
    MOCK_EXPECT( functor.Visit ).once().with( "Callsign", boost::bind( &CheckSize, _1, sizeof( uint32_t ) + 4 * sizeof( uint16_t ) ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_higher_headquarters_uniqueid_is_its_own_identifier, AggregateFixture )
{
    MOCK_EXPECT( functor.Visit ).once().with( "HigherHeadquarters", boost::bind( &CheckSize, _1, 11u ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_echelon_is_platoon, AggregateFixture )
{
    const int8_t platoonEchelon = 14;
    hla::MockUpdateFunctor functor;
    MOCK_EXPECT( functor.Visit ).once().with( "Echelon", boost::bind( &CheckSerialization< int8_t >, _1, platoonEchelon ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( agent_not_mounted_serializes_0_percent, AggregateFixture )
{
    const double percentMounted = 0.;
    const int8_t active = 1;
    MOCK_EXPECT( functor.Visit ).once().with( "Mounted", boost::bind( &CheckSerialization< double >, _1, percentMounted ) );
    MOCK_EXPECT( functor.Visit ).once().with( "Status", boost::bind( &CheckSerialization< int8_t >, _1, active ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( mounted_agent_serializes_100_percent, AggregateFixture )
{
    const double percentMounted = 100.;
    const int8_t inactive = 2;
    BOOST_REQUIRE( listener );
    listener->EmbarkmentChanged( true );
    MOCK_EXPECT( functor.Visit ).once().with( "Mounted", boost::bind( &CheckSerialization< double >, _1, percentMounted ) );
    MOCK_EXPECT( functor.Visit ).once().with( "Status", boost::bind( &CheckSerialization< int8_t >, _1, inactive ) );
    MOCK_EXPECT( functor.Visit );
    entity.Serialize( functor, true );
}
