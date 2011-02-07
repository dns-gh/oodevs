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
#include "hla_plugin/AgentListener_ABC.h"
#include "MockAgentSubject.h"
#include "MockRtiAmbassadorFactory.h"
#include "MockAgent.h"
#include "MockRtiAmbassador.h" // $$$$ _RC_ SLI 2011-01-11: Don't mock type you don't own
#include <hla/HLA_lib.h>
#include <hla/SimpleTime.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace hla
{
    std::ostream& operator<<( std::ostream& os, const hla::ClassIdentifier& identifier )
    {
        return os << identifier.ToString();
    }
}

BOOST_AUTO_TEST_CASE( hla_plugin_initialization_declares_publications )
{
    xml::xistringstream xis( "<root name='name' federation='federation'/>" );
    xis >> xml::start( "root" );
    MockAgentSubject subject;
    MockRtiAmbassadorFactory factory;
    hla::MockRtiAmbassador* rtiAmbassador( new hla::MockRtiAmbassador() );
    hla::FederateAmbassador_ABC* federateAmbassador = 0;
    hla::SimpleTime target;
    mock::sequence s;
    MOCK_EXPECT( factory, CreateAmbassador ).once().in( s ).with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "localhost", "8989" ).returns( std::auto_ptr< hla::RtiAmbassador_ABC >( rtiAmbassador ) );
    MOCK_EXPECT( rtiAmbassador, Join ).once().in( s ).with( "name", "federation", mock::retrieve( federateAmbassador ) ).returns( true );
    MOCK_EXPECT( rtiAmbassador, Tick ).calls( boost::bind( &hla::FederateAmbassador_ABC::TimeAdvanceGranted, boost::ref( federateAmbassador ), boost::ref( target ) ) );;
    MOCK_EXPECT( rtiAmbassador, EnableTimeRegulation ).once().in( s ).calls( boost::bind( &hla::FederateAmbassador_ABC::TimeRegulationEnabled, boost::ref( federateAmbassador ), boost::ref( target ) ) );
    MOCK_EXPECT( rtiAmbassador, PublishClass ).once().in( s ).with( "BaseEntity.AggregateEntity", mock::any );
    MOCK_EXPECT( subject, Register ).once().in( s );
    FederateFacade facade( xis, subject, factory, 0u );
    MOCK_EXPECT( subject, Unregister ).once().in( s );
    MOCK_EXPECT( rtiAmbassador, Resign ).once().in( s );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis               ( "<root name='name' federation='federation'/>" )
            , rtiAmbassador     ( new hla::MockRtiAmbassador() )
            , federateAmbassador( 0 )
            , listener          ( 0 )
        {
            MOCK_EXPECT( factory, CreateAmbassador ).once().with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "localhost", "8989" ).returns( std::auto_ptr< hla::RtiAmbassador_ABC >( rtiAmbassador ) );
            MOCK_EXPECT( rtiAmbassador, Join ).once().with( "name", "federation", mock::retrieve( federateAmbassador ) ).returns( true );
            MOCK_EXPECT( rtiAmbassador, Tick ).calls( boost::bind( &hla::FederateAmbassador_ABC::TimeAdvanceGranted, boost::ref( federateAmbassador ), boost::ref( target ) ) );;
            MOCK_EXPECT( rtiAmbassador, EnableTimeRegulation ).once().calls( boost::bind( &hla::FederateAmbassador_ABC::TimeRegulationEnabled, boost::ref( federateAmbassador ), boost::ref( target ) ) );
            MOCK_EXPECT( rtiAmbassador, PublishClass ).once().with( "BaseEntity.AggregateEntity", mock::any );
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( subject, Unregister ).once();
            MOCK_EXPECT( rtiAmbassador, Resign ).once();
        }
        xml::xistringstream xis;
        MockAgentSubject subject;
        MockRtiAmbassadorFactory factory;
        hla::MockRtiAmbassador* rtiAmbassador;
        hla::FederateAmbassador_ABC* federateAmbassador;
        hla::SimpleTime target;
        AgentListener_ABC* listener;
    };
}

BOOST_FIXTURE_TEST_CASE( hla_plugin_publishes_agent_instance, Fixture )
{
    MockAgent agent;
    FederateFacade facade( xis >> xml::start( "root" ), subject, factory, 0u );
    BOOST_REQUIRE( listener );
    mock::sequence s;
    MOCK_EXPECT( agent, Register ).once().in( s );
    MOCK_EXPECT( rtiAmbassador, ReserveObjectInstance ).once().in( s ).with( "id", mock::any ).calls( boost::bind( &hla::FederateAmbassador_ABC::ReservationSucceded, boost::ref( federateAmbassador ) ) );;
    MOCK_EXPECT( rtiAmbassador, RegisterObjectInstance ).once().in( s ).with( "BaseEntity.AggregateEntity", "id" ).returns( hla::ObjectIdentifier( 42u ) );
    listener->Created( agent, "id", "name", rpr::Friendly );
    MOCK_EXPECT( agent, Unregister ).once().in( s );
    MOCK_EXPECT( rtiAmbassador, DeleteObjectInstance ).once().in( s ).with( hla::ObjectIdentifier( 42u ) );
}
