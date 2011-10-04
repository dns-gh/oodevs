// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/ClassBuilder.h"
#include "hla_plugin/HlaObject_ABC.h"
#include <hla/Class.h>  // $$$$ _RC_ SLI 2011-07-26: not mockable
#include "MockFederate.h"
#include "MockObjectRegistration.h" // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include "MockAttributeFunctor.h"   // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : hlaClass( objectRegistration, true )
        {}
        ClassBuilder builder;
        MockFederate federate;
        ::hla::MockObjectRegistration< HlaObject_ABC > objectRegistration;
        ::hla::Class< HlaObject_ABC > hlaClass;
    };
}

BOOST_FIXTURE_TEST_CASE( builder_registers_rpr_aggregate_class_identifier_and_forwards_subscriptions, Fixture )
{
    MOCK_EXPECT( federate, RegisterClass ).once().with( "BaseEntity.AggregateEntity", mock::any, true, false );
    builder.BuildAggregate( federate, hlaClass, true, false );
}

BOOST_FIXTURE_TEST_CASE( builder_registers_only_rpr_aggregate_attributes, Fixture )
{
    MOCK_EXPECT( federate, RegisterClass );
    builder.BuildAggregate( federate, hlaClass, true, false );
    mock::verify();
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
                                                                        ( "SilentAggregates" )
                                                                        ( "SubAggregateIdentifiers" )
                                                                        ( "EntityIdentifiers" );
    mock::sequence s;
    ::hla::MockAttributeFunctor visitor;
    BOOST_FOREACH( const std::string& attribute, attributes )
        MOCK_EXPECT( visitor, Visit ).once().in( s ).with( attribute );
    hlaClass.Apply( visitor );
}
