// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/TransportationController.h"
#include "MockMissionResolver.h"
#include "MockTransportationListener.h"
#include "tools/MessageController.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( transportation_controller_reads_transportation_mission_name_from_xml )
{
    xml::xistringstream xis( "<configuration>"
                             "    <missions>"
                             "        <transport>transportation mission name</transport>"
                             "    </missions>"
                             "</configuration>" );
    xis >> xml::start( "configuration" );
    tools::MessageController< sword::SimToClient_Content > messageController;
    MockMissionResolver resolver;
    MOCK_EXPECT( resolver, Resolve ).once().with( "transportation mission name" ).returns( 42 );
    TransportationController controller( xis, resolver, messageController );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis( "<configuration>"
                   "    <missions>"
                   "        <transport>name</transport>"
                   "    </missions>"
                   "</configuration>" )
            , transportId( 42 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( resolver, Resolve ).once().returns( transportId );
        }
        sword::SimToClient_Content MakeTransportationMessage( unsigned int missionType )
        {
            sword::SimToClient_Content message;
            sword::AutomatOrder* automatOrder = message.mutable_automat_order();
            automatOrder->mutable_type()->set_id( missionType );
            return message;
        }
        xml::xistringstream xis;
        unsigned int transportId;
        MockMissionResolver resolver;
        MockTransportationListener listener;
        tools::MessageController< sword::SimToClient_Content > messageController;
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_listens_to_transportation_mission_and_notifies_listener, Fixture )
{
    TransportationController controller( xis, resolver, messageController );
    controller.Register( listener );
    MOCK_EXPECT( listener, ConvoyRequested ).once();
    messageController.Dispatch( MakeTransportationMessage( transportId ) );
}

BOOST_FIXTURE_TEST_CASE( transporation_controller_does_nothing_if_mission_is_not_transportation, Fixture )
{
    TransportationController controller( xis, resolver, messageController );
    controller.Register( listener );
    const unsigned int unknownMission = transportId + 1;
    messageController.Dispatch( MakeTransportationMessage( unknownMission ) );
}
