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
#include "MockHlaObject.h"
#include "MockUpdateFunctor.h"
#include <hla/Deserializer.h>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : rprRemote( new MockHlaObject() )
            , pRemote  ( static_cast< HlaObject_ABC* >( rprRemote ) )
        {}
        MockHlaObject* rprRemote;
        std::auto_ptr< HlaObject_ABC > pRemote;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_deserialize_also_rpr_remote_aggregate, Fixture )
{
    NetnRemoteAggregate netnRemote( pRemote );
    mock::sequence s;
    MOCK_EXPECT( rprRemote, Deserialize ).once().in( s );
    const int8 buffer = 0;
    const ::hla::Deserializer deserializer( &buffer );
    netnRemote.Deserialize( "attribute", deserializer );
}
BOOST_FIXTURE_TEST_CASE( netn_remote_aggregate_cannot_be_serialized, Fixture )
{
    NetnRemoteAggregate netnRemote( pRemote );
    ::hla::MockUpdateFunctor functor;
    BOOST_CHECK_THROW( netnRemote.Serialize( functor, true ), std::runtime_error );
}
