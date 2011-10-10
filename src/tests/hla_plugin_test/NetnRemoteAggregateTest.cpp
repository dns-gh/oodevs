// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnRemoteAggregate.h"
#include "hla_plugin/UnicodeString.h"
#include "hla_plugin/UniqueId.h"
#include "MockHlaObject.h"
#include "MockUpdateFunctor.h"
#include "MockRemoteAgentListener.h"
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

namespace
{
    typedef std::vector< int8 > T_Buffer;
    class Fixture
    {
    public:
        Fixture()
            : rprRemote ( new MockHlaObject() )
            , pRemote   ( static_cast< HlaObject_ABC* >( rprRemote ) )
            , netnRemote( pRemote, listener, "identifier" )
        {}
        ::hla::Deserializer Deserialize()
        {
            buffer.resize( serializer.GetSize() );
            serializer.CopyTo( &buffer[0] );
            return ::hla::Deserializer( &buffer[0], buffer.size() );
        }
        MockHlaObject* rprRemote;
        std::auto_ptr< HlaObject_ABC > pRemote;
        MockRemoteAgentListener listener;
        NetnRemoteAggregate netnRemote;
        ::hla::Serializer serializer;
        T_Buffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserialize_also_rpr_remote_aggregate, Fixture )
{
    mock::sequence s;
    MOCK_EXPECT( rprRemote, Deserialize ).once().in( s );
    const int8 buffer = 0;
    const ::hla::Deserializer deserializer( &buffer );
    netnRemote.Deserialize( "attribute", deserializer );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_cannot_be_serialized, Fixture )
{
    ::hla::MockUpdateFunctor functor;
    BOOST_CHECK_THROW( netnRemote.Serialize( functor, true ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserializes_callsign_attribute_and_notifies_listener, Fixture )
{
    const UnicodeString callsign( "callsign" );
    callsign.Serialize( serializer );
    MOCK_EXPECT( rprRemote, Deserialize );
    MOCK_EXPECT( listener, CallsignChanged ).once().with( "identifier", "callsign" );
    netnRemote.Deserialize( "Callsign", Deserialize() );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserializes_unique_identifier_attribute_and_notifies_listener, Fixture )
{
    const UniqueId uniqueId( "unique" );
    uniqueId.Serialize( serializer );
    MOCK_EXPECT( rprRemote, Deserialize );
    MOCK_EXPECT( listener, UniqueIdChanged ).once().with( "identifier", "unique" );
    netnRemote.Deserialize( "UniqueID", Deserialize() );
}
