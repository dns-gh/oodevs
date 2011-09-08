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
#include "hla_plugin/SerializationTools.h"
#include "MockUpdateFunctor.h"
#include "MockRemoteAgentListener.h"
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <vector>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( remote_aggregate_cannot_be_serialized )
{
    MockRemoteAgentListener listener;
    RemoteAggregate aggregate( "identifier", listener );
    ::hla::MockUpdateFunctor functor;
    BOOST_CHECK_THROW( aggregate.Serialize( functor, true ), std::runtime_error );
}

namespace
{
    typedef std::vector< int8 > T_Buffer;
}

BOOST_AUTO_TEST_CASE( remote_aggregate_deserializes_spatial_attribute_and_notifies_listener )
{
    MockRemoteAgentListener listener;
    RemoteAggregate aggregate( "identifier", listener );
    Spatial spatial( true, 1., 2., 3., 4., 5. );
    ::hla::Serializer serializer;
    spatial.Serialize( serializer );
    T_Buffer buffer( serializer.GetSize() );
    serializer.CopyTo( &buffer[0] );
    const ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    MOCK_EXPECT( listener, Moved ).once().with( "identifier", mock::close( 1., 0.001 ), mock::close( 2., 0.001 ) );
    aggregate.Deserialize( "Spatial", deserializer );
}

BOOST_AUTO_TEST_CASE( remote_aggregate_deserializes_force_identifier_attribute_and_notifies_listener )
{
    MockRemoteAgentListener listener;
    RemoteAggregate aggregate( "identifier", listener );
    ::hla::Serializer serializer;
    serializer << static_cast< int8 >( rpr::Friendly );
    T_Buffer buffer( serializer.GetSize() );
    serializer.CopyTo( &buffer[0] );
    const ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    MOCK_EXPECT( listener, SideChanged ).once().with( "identifier", rpr::Friendly );
    aggregate.Deserialize( "ForceIdentifier", deserializer );
}
