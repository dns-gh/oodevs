// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/TransportationFacade.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnitsVisitor_ABC.h"
#include "hla_plugin/InteractionBuilder.h"
#include "MockInteractionSender.h"
#include "MockContextFactory.h"
#include "MockMissionResolver.h"
#include "MockCallsignResolver.h"
#include "MockClientPublisher.h"
#include "MockSimulationPublisher.h"
#include "MockLogger.h"
#include "MockFederate.h"
#include "MockNETNInteractionBuilder.h"
#include "MockSubordinates.h"
#include "tools/MessageController.h"
#include "protocol/Simulation.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis        ( "<configuration>"
                           "    <missions>"
                           "        <fragOrders>"
                           "            <cancel>cancel</cancel>"
                           "            <pause>pause</pause>"
                           "            <resume>resume</resume>"
                           "        </fragOrders>"
                           "        <offer>"
                           "            <transport>transport_offer</transport>"
                           "            <embarkment>embarkment_offer</embarkment>"
                           "            <disembarkment>disembarkment_offer</disembarkment>"
                           "        </offer>"
                           "        <request>"
                           "            <transport>transport_request</transport>"
                           "            <embarkment>embarkment_request</embarkment>"
                           "            <disembarkment>disembarkment_request</disembarkment>"
                           "        </request>"
                           "    </missions>"
                           "    <reports>"
                           "        <mission-complete>1338</mission-complete>"
                           "    </reports>"
                           "</configuration>" )
            , transportId( 42 )
            , embarkId( 43 )
            , disembarkId( 44 )
            , cancelId ( 45 )
            , pauseId ( 46 )
            , resumeId ( 47 )
            ,interactionBuilder( logger, federate, nETNInteractionBuilder )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnRequestConvoy ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnOfferConvoy ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnAcceptConvoy ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnRejectOfferConvoy ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnCancelConvoy ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnReadyToReceiveService ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnServiceStarted ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnConvoyEmbarkmentStatus ).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnConvoyDisembarkmentStatus).once().returns( true );
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnConvoyDestroyedEntities ).once().returns( true );;
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnServiceComplete ).once().returns( true );;
            MOCK_EXPECT( nETNInteractionBuilder.BuildNetnServiceReceived ).once().returns( true );
        }
        xml::xistringstream xis;
        unsigned int transportId;
        unsigned int embarkId;
        unsigned int disembarkId;
        unsigned int cancelId;
        unsigned int pauseId;
        unsigned int resumeId;
        dispatcher::MockLogger logger;
        MockMissionResolver missionResolver;
        tools::MessageController< sword::SimToClient_Content > messageController;
        MockFederate federate;
        MockContextFactory contextFactory;
        MockCallsignResolver callsignResolver;
        dispatcher::MockClientPublisher clientPublisher;
        dispatcher::MockSimulationPublisher simulationPublisher;
        MockNETNInteractionBuilder nETNInteractionBuilder;
        MockSubordinates subordinates;
        InteractionBuilder interactionBuilder;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_transportation_facade_init, Fixture )
{
    std::unique_ptr< TransportationFacade > transportationFacade;

    // requester
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "pause" ).returns( pauseId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "resume" ).returns( resumeId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "cancel" ).returns( cancelId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "transport_request" ).returns( transportId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport_request" ).returns( transportId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "embarkment_request" ).returns( embarkId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "embarkment_request" ).returns( embarkId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "disembarkment_request" ).returns( disembarkId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "disembarkment_request" ).returns( disembarkId );
    // offerer
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "cancel" ).returns( cancelId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport_offer" ).returns( transportId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "embarkment_offer" ).returns( embarkId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "disembarkment_offer" ).returns( disembarkId );

    BOOST_CHECK_NO_THROW( transportationFacade.reset( new TransportationFacade ( logger, xis, missionResolver, messageController, callsignResolver, subordinates,
            interactionBuilder, contextFactory, simulationPublisher, clientPublisher) ) );
}

BOOST_FIXTURE_TEST_CASE( netn_transportation_facade_invalid_pause, Fixture )
{
    std::unique_ptr< TransportationFacade > transportationFacade;

    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "pause" ).throws( tools::Exception( "file", "function", 1, "frag order pause does not exist") );

    MOCK_EXPECT( logger.LogError ).once();

    BOOST_CHECK_NO_THROW( transportationFacade.reset( new TransportationFacade ( logger, xis, missionResolver, messageController, callsignResolver, subordinates,
            interactionBuilder, contextFactory, simulationPublisher, clientPublisher) ) );
}

BOOST_FIXTURE_TEST_CASE( netn_transportation_facade_invalid_transport_request, Fixture )
{
    std::unique_ptr< TransportationFacade > transportationFacade;

    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "pause" ).returns( pauseId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "resume" ).returns( resumeId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "cancel" ).returns( cancelId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "transport_request" ).throws( tools::Exception( "file", "function", 1, "mission transport_request does not exist") );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport_request" ).throws( tools::Exception( "file", "function", 1, "mission transport_request does not exist") );

    MOCK_EXPECT( logger.LogError ).once();

    BOOST_CHECK_NO_THROW( transportationFacade.reset( new TransportationFacade ( logger, xis, missionResolver, messageController, callsignResolver, subordinates,
            interactionBuilder, contextFactory, simulationPublisher, clientPublisher) ) );
}

BOOST_FIXTURE_TEST_CASE( netn_transportation_facade_invalid_transport_offer, Fixture )
{
    std::unique_ptr< TransportationFacade > transportationFacade;

    // requester
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "pause" ).returns( pauseId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "resume" ).returns( resumeId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "cancel" ).returns( cancelId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "transport_request" ).returns( transportId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport_request" ).returns( transportId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "embarkment_request" ).returns( embarkId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "embarkment_request" ).returns( embarkId );
    MOCK_EXPECT( missionResolver.ResolveAutomat ).once().with( "disembarkment_request" ).returns( disembarkId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "disembarkment_request" ).returns( disembarkId );
    // offerer
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "cancel" ).returns( cancelId );
    MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport_offer" ).throws( tools::Exception( "file", "function", 1, "mission transport_offer does not exist") );

    MOCK_EXPECT( logger.LogError ).once();


    BOOST_CHECK_NO_THROW( transportationFacade.reset( new TransportationFacade ( logger, xis, missionResolver, messageController, callsignResolver, subordinates,
            interactionBuilder, contextFactory, simulationPublisher, clientPublisher) ) );
}
