// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AggregateEntityClass.h"
#include "MockAgentSubject.h"
#include "MockRtiAmbassador.h"
#include <hla/Federate.h>
#include <hla/SimpleTime.h>
#include <hla/SimpleTimeInterval.h>
#include <hla/AttributeFunctor_ABC.h>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

namespace
{
    MOCK_BASE_CLASS( MockAttributeFunctor, hla::AttributeFunctor_ABC )
    {
        MOCK_METHOD( Visit, 1 );
    };

    class Fixture
    {
    public:
        Fixture()
            : federate( ambassador, "name", hla::SimpleTime(), hla::SimpleTimeInterval() )
        {
            MOCK_EXPECT( ambassador, Tick ).once();
            MOCK_EXPECT( ambassador, Connect ).once().returns( true );
            MOCK_EXPECT( ambassador, Disconnect ).once();
            federate.Connect();
            MOCK_EXPECT( ambassador, Join ).once().returns( true );
            MOCK_EXPECT( ambassador, Resign ).once();
            federate.Join( "federation", false, false );
            MOCK_EXPECT( subject, Register ).once();
            MOCK_EXPECT( subject, Unregister ).once();
        }
        hla::MockRtiAmbassador ambassador;
        hla::Federate federate;
        plugins::hla::MockAgentSubject subject;
    };
    bool CheckAttributes( const hla::Class_ABC& hlaClass, const std::vector< std::string >& attributes )
    {
        MockAttributeFunctor visitor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( visitor, Visit ).once().in( s ).with( attribute );
        hlaClass.Apply( visitor );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( aggregate_entity_class_registers_attributes, Fixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "EntityType" )
                                                                        ( "EntityIdentifier" )
                                                                        ( "Spatial" )
                                                                        ( "AggregateMarking" )
                                                                        ( "AggregateState" )
                                                                        ( "Dimensions" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "Formation" )
                                                                        ( "NumberOfSilentEntities" )
                                                                        ( "SilentEntities" );
    MOCK_EXPECT( ambassador, PublishClass ).once().with( "BaseEntity.AggregateEntity", boost::bind( &CheckAttributes, _1, attributes ) );
    plugins::hla::AggregateEntityClass entity( federate, subject );
}
