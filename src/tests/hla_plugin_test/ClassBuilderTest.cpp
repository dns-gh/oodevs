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
#include "hla_plugin/Aggregate_ABC.h"
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
        ::hla::MockObjectRegistration< Aggregate_ABC > objectRegistration;
        ::hla::Class< Aggregate_ABC > hlaClass;
    };
}

BOOST_FIXTURE_TEST_CASE( builder_registers_rpr_class_identifier_and_publish_only, Fixture )
{
    MOCK_EXPECT( federate, RegisterClass ).once().with( "BaseEntity.AggregateEntity", mock::any, true, false );
    builder.Build( federate, hlaClass );
}

BOOST_FIXTURE_TEST_CASE( builder_registers_only_rpr_attributes, Fixture )
{
    MOCK_EXPECT( federate, RegisterClass );
    builder.Build( federate, hlaClass );
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
