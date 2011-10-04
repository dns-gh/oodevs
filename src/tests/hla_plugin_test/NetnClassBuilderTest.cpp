// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnClassBuilder.h"
#include "hla_plugin/HlaObject_ABC.h"
#include <hla/Class.h>  // $$$$ _RC_ SLI 2011-07-26: not mockable
#include "MockClassBuilder.h"
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
            : builder( mockBuilder )
            , hlaClass( objectRegistration, true )
        {}
        MockClassBuilder mockBuilder;
        NetnClassBuilder builder;
        MockFederate federate;
        ::hla::MockObjectRegistration< HlaObject_ABC > objectRegistration;
        ::hla::Class< HlaObject_ABC > hlaClass;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_builder_registers_netn_class_identifier_and_forwards_subscriptions, Fixture )
{
    mock::sequence s;
    MOCK_EXPECT( mockBuilder, Build ).once().in( s );
    MOCK_EXPECT( federate, RegisterClass ).once().in( s ).with( "BaseEntity.AggregateEntity.NETN_Aggregate", mock::any, true, false );
    builder.Build( federate, hlaClass, true, false );
}

BOOST_FIXTURE_TEST_CASE( netn_builder_registers_only_netn_attributes, Fixture )
{
    MOCK_EXPECT( mockBuilder, Build );
    MOCK_EXPECT( federate, RegisterClass );
    builder.Build( federate, hlaClass, true, true );
    mock::verify();
    const std::vector< std::string > attributes = boost::assign::list_of( "Mounted" )
                                                                        ( "Echelon" )
                                                                        ( "UniqueID" )
                                                                        ( "HigherHeadquarters" )
                                                                        ( "Callsign" );
    mock::sequence s;
    ::hla::MockAttributeFunctor visitor;
    BOOST_FOREACH( const std::string& attribute, attributes )
        MOCK_EXPECT( visitor, Visit ).once().in( s ).with( attribute );
    hlaClass.Apply( visitor );
}
