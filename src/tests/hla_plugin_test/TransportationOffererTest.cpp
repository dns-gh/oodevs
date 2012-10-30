// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/TransportationOfferer.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnitsVisitor_ABC.h"
#include "MockInteractionSender.h"
#include "MockContextFactory.h"
#include "MockMissionResolver.h"
#include "MockCallsignResolver.h"
#include "MockClientPublisher.h"
#include "MockSimulationPublisher.h"
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
                           "        </fragOrders>"
                           "        <offer>"
                           "            <transport>transport</transport>"
                           "            <embarkment>embarkment</embarkment>"
                           "            <disembarkment>disembarkment</disembarkment>"
                           "        </offer>"
                           "    </missions>"
                           "    <reports>"
                           "        <mission-complete>1338</mission-complete>"
                           "    </reports>"
                           "</configuration>" )
            , transportId( 42 )
            , embarkId( 43 )
            , disembarkId( 44 )
            , cancelId ( 45 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "transport" ).returns( transportId );
            MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "embarkment" ).returns( embarkId );
            MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "disembarkment" ).returns( disembarkId );
            MOCK_EXPECT( missionResolver.ResolveUnit ).once().with( "cancel" ).returns( cancelId );
        }
        xml::xistringstream xis;
        unsigned int transportId;
        unsigned int embarkId;
        unsigned int disembarkId;
        unsigned int cancelId;
        MockMissionResolver missionResolver;
        MockInteractionSender< interactions::NetnOfferConvoy > offerInteractionSender;
        MockInteractionSender< interactions::NetnServiceStarted > serviceStartedInteractionSender;
        MockInteractionSender< interactions::NetnConvoyEmbarkmentStatus > convoyEmbarkmentStatusInteractionSender;
        MockInteractionSender< interactions::NetnConvoyDisembarkmentStatus > convoyDisembarkmentStatusInteractionSender;
        MockInteractionSender< interactions::NetnConvoyDestroyedEntities > convoyDestroyedEntitiesSender;
        MockInteractionSender< interactions::NetnServiceComplete > serviceCompleteSender;
        MockInteractionSender< interactions::NetnCancelConvoy > cancelConvoySender;
        interactions::NetnOfferConvoy offer;
        interactions::NetnRequestConvoy request;
        tools::MessageController< sword::SimToClient_Content > messageController;
        MockContextFactory contextFactory;
        MockCallsignResolver callsignResolver;
        dispatcher::MockClientPublisher clientPublisher;
        dispatcher::MockSimulationPublisher simulationPublisher;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_sender_sends_announce_offer_to_clients_and_creates_flags_at_embarking_and_debarking_points, Fixture )
{
    TransportationOfferer transportationOfferer( xis, missionResolver, offerInteractionSender, serviceStartedInteractionSender,
                                                 convoyEmbarkmentStatusInteractionSender, convoyDisembarkmentStatusInteractionSender, convoyDestroyedEntitiesSender, serviceCompleteSender, cancelConvoySender,
                                                 messageController, contextFactory, callsignResolver, clientPublisher, simulationPublisher );
    request.serviceType = 4; // Convoy
    request.consumer = UnicodeString( "consumer" );
    request.provider = UnicodeString( "provider" );
    request.serviceId.eventCount = 42;
    request.serviceId.issuingObjectIdentifier = UnicodeString( "objectIdentifier" );
    request.transportData.convoyType = NetnTransportStruct::E_Transport;
    request.transportData.dataTransport.appointment = NetnAppointmentStruct( 1, rpr::WorldLocation( 1., 2., 3. ) );
    request.transportData.dataTransport.finalAppointment = NetnAppointmentStruct( 2, rpr::WorldLocation( 4., 5., 6. ) );
    request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", MakeUniqueId( "unique" ), NetnObjectFeatureStruct() ) );
    sword::MessengerToClient announce;
    sword::MessengerToClient flag;
    MOCK_EXPECT( clientPublisher.SendMessengerToClient ).once().with( mock::retrieve( announce ) );
    MOCK_EXPECT( contextFactory.Create ).exactly( 2 ).returns( 42 );
    MOCK_EXPECT( clientPublisher.SendMessengerToClient ).exactly( 2 ).with( mock::retrieve( flag ) );
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().with( MakeUniqueId( "unique" ) ).returns( 111 );
    transportationOfferer.Receive( request );
    mock::verify();
    BOOST_CHECK( announce.message().has_text_message() );
    BOOST_CHECK( flag.message().has_shape_creation() );
}

// TODO add protocol test
