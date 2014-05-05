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
#include "hla_plugin/UnicodeString.h"
#include "hla_plugin/UniqueId.h"
#include "MockHlaObject.h"
#include "MockUpdateFunctor.h"
#include "MockObjectListener.h"
#include "hla_plugin/FOM_Serializer.h"
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

namespace
{
    typedef std::vector< uint8_t > T_Buffer;
    class Fixture
    {
    public:
        Fixture()
            : rprRemote ( new MockHlaObject() )
            , fomSerialization( 1 )
            , netnRemote( std::unique_ptr< HlaObject_ABC >( rprRemote ), "identifier", fomSerialization )
        {
            MOCK_EXPECT( rprRemote->Register );
            netnRemote.Register( listener );
        }
        ::hla::Deserializer Deserialize()
        {
            buffer.resize( serializer.GetSize() );
            serializer.CopyTo( &buffer[0] );
            return ::hla::Deserializer( &buffer[0], buffer.size() );
        }
        MockHlaObject* rprRemote;
        MockObjectListener listener;
        FOM_Serializer fomSerialization;
        NetnAggregate netnRemote;
        ::hla::Serializer serializer;
        T_Buffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserialize_also_rpr_remote_aggregate, Fixture )
{
    mock::sequence s;
    MOCK_EXPECT( rprRemote->Deserialize ).once().in( s );
    const uint8_t buffer = 0;
    ::hla::Deserializer deserializer( &buffer );
    netnRemote.Deserialize( "attribute", deserializer );
}

BOOST_FIXTURE_TEST_CASE( empty_netn_remote_aggregate_can_be_serialized, Fixture )
{
    ::hla::MockUpdateFunctor functor;
    MOCK_EXPECT( functor.Visit );
    MOCK_EXPECT( rprRemote->Serialize );
    netnRemote.Serialize( functor, true );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserializes_callsign_attribute_and_notifies_listener, Fixture )
{
    const UnicodeString callsign( "callsign" );
    callsign.Serialize( serializer );
    MOCK_EXPECT( rprRemote->Deserialize );
    MOCK_EXPECT( listener.CallsignChanged ).once().with( "identifier", "callsign" );
    ::hla::Deserializer deserializer( Deserialize() );
    netnRemote.Deserialize( "Callsign", deserializer );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserializes_unique_identifier_attribute_and_notifies_listener, Fixture )
{
    const UniqueId uniqueId( MakeUniqueId( "unique" ) );
    uniqueId.Serialize( serializer );
    MOCK_EXPECT( rprRemote->Deserialize );
    MOCK_EXPECT( listener.UniqueIdChanged ).once().with( "identifier", MakeUniqueId( "unique" ) );
    ::hla::Deserializer deserializer( Deserialize() );
    netnRemote.Deserialize( "UniqueID", deserializer );
}
