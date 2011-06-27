// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/FederateFacade.h"
#include "protocol/Simulation.h"
#include "MockAgentSubject.h"
#include "MockRtiAmbassadorFactory.h"
#include "MockFederateAmbassadorFactory.h"
#include "MockFederate.h"
#include "MockRtiAmbassador.h"
#include "MockMessageDispatcher.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : federate( new MockFederate() )
            , listener( 0 )
        {}
        MockAgentSubject subject;
        MockRtiAmbassadorFactory rtiFactory;
        MockFederateAmbassadorFactory federateFactory;
        MockFederate* federate;
        MockMessageDispatcher< sword::SimToClient_Content > controller;
        MessageHandler_ABC< sword::SimToClient_Content >* listener;
        mock::sequence s;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_initialization_declares_publications, Fixture )
{
    xml::xistringstream xis( "<root/>" );
    xis >> xml::start( "root" );
    MOCK_EXPECT( rtiFactory, CreateAmbassador ).once().in( s ).with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "localhost", "8989" ).returns( std::auto_ptr< hla::RtiAmbassador_ABC >( new ::hla::MockRtiAmbassador() ) );
    MOCK_EXPECT( federateFactory, Create ).once().in( s ).with( mock::any, "SWORD", -1 ).returns( std::auto_ptr< Federate_ABC >( federate ) );
    MOCK_EXPECT( federate, Connect ).once().in( s ).returns( true );
    MOCK_EXPECT( federate, Join ).once().in( s ).with( "Federation", true, true ).returns( true );
    MOCK_EXPECT( federate, RegisterClass ).once().in( s );
    MOCK_EXPECT( subject, Register ).once().in( s );
    MOCK_EXPECT( controller, Register ).once().in( s );
    FederateFacade facade( xis, controller, subject, rtiFactory, federateFactory, "directory" );
    MOCK_EXPECT( subject, Unregister ).once().in( s );
    MOCK_EXPECT( controller, Unregister ).once().in( s );
}

namespace
{
    class BuildFixture : public Fixture
    {
    public:
        BuildFixture()
        {
            MOCK_EXPECT( subject, Register ).once();
            MOCK_EXPECT( subject, Unregister ).once();
            MOCK_EXPECT( controller, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( controller, Unregister ).once();
            MOCK_EXPECT( federate, RegisterClass ).once();
            MOCK_EXPECT( federate, Connect ).once().returns( true );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_xml_options_overrides_default_values, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' federation='federation' host='host' port='1337'"
                             "      time-constrained='false' time-regulating='false' lookahead='3'/>" );
    xis >> xml::start( "root" );
    MOCK_EXPECT( rtiFactory, CreateAmbassador ).once().with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "host", "1337" ).returns( std::auto_ptr< hla::RtiAmbassador_ABC >( new ::hla::MockRtiAmbassador() ) );
    MOCK_EXPECT( federateFactory, Create ).once().with( mock::any, "name", 3 ).returns( std::auto_ptr< Federate_ABC >( federate ) );
    MOCK_EXPECT( federate, Join ).once().with( "federation", false, false ).returns( true );
    FederateFacade facade( xis, controller, subject, rtiFactory, federateFactory, "directory" );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_create_federation, BuildFixture )
{
    xml::xistringstream xis( "<root name='name' creation='true' fom='fom'/>" );
    xis >> xml::start( "root" );
    MOCK_EXPECT( rtiFactory, CreateAmbassador ).once().returns( std::auto_ptr< hla::RtiAmbassador_ABC >( new ::hla::MockRtiAmbassador() ) );
    MOCK_EXPECT( federateFactory, Create ).once().returns( std::auto_ptr< Federate_ABC >( federate ) );
    MOCK_EXPECT( federate, Join ).once().in( s ).returns( false );
    MOCK_EXPECT( federate, Create ).once().in( s ).with( "Federation", "directory/fom" ).returns( true );
    MOCK_EXPECT( federate, Join ).once().in( s ).returns( true );
    FederateFacade facade( xis, controller, subject, rtiFactory, federateFactory, "directory" );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_can_destroy_federation, BuildFixture )
{
    xml::xistringstream xis( "<root destruction='true'/>" );
    xis >> xml::start( "root" );
    MOCK_EXPECT( rtiFactory, CreateAmbassador ).once().returns( std::auto_ptr< hla::RtiAmbassador_ABC >( new ::hla::MockRtiAmbassador() ) );
    MOCK_EXPECT( federateFactory, Create ).once().returns( std::auto_ptr< Federate_ABC >( federate ) );
    MOCK_EXPECT( federate, Join ).once().returns( true );
    FederateFacade facade( xis, controller, subject, rtiFactory, federateFactory, "directory" );
    MOCK_EXPECT( federate, Destroy ).once().with( "Federation" ).returns( true );
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_steps, BuildFixture )
{
    xml::xistringstream xis( "<root/>" );
    xis >> xml::start( "root" );
    MOCK_EXPECT( rtiFactory, CreateAmbassador ).once().returns( std::auto_ptr< hla::RtiAmbassador_ABC >( new ::hla::MockRtiAmbassador() ) );
    MOCK_EXPECT( federateFactory, Create ).once().returns( std::auto_ptr< Federate_ABC >( federate ) );
    MOCK_EXPECT( federate, Join ).once().returns( true );
    FederateFacade facade( xis, controller, subject, rtiFactory, federateFactory, "directory" );
    BOOST_REQUIRE( listener );
    MOCK_EXPECT( federate, Step ).once();
    sword::SimToClient_Content message;
    message.mutable_control_end_tick()->set_current_tick( 3 );
    listener->Notify( message );
}
