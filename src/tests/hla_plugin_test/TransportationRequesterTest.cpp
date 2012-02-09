// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/TransportationRequester.h"
#include "hla_plugin/TransportedUnits_ABC.h"
#include "hla_plugin/Interactions.h"
#include "MockMissionResolver.h"
#include "MockCallsignResolver.h"
#include "MockTransportedUnitsVisitor.h"
#include "MockSubordinates.h"
#include "MockContextFactory.h"
#include "MockSimulationPublisher.h"
#include "MockTransportedUnits.h"
#include "MockInteractionSender.h"
#include "tools/MessageController.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>
#include <boost/assign.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( transportation_requester_reads_transportation_mission_name_from_xml )
{
    xml::xistringstream xis( "<configuration>"
                             "    <missions>"
                             "        <fragOrders>"
                             "            <pause>pause</pause>"
                             "            <resume>resume</resume>"
                             "            <cancel>cancel</cancel>"
                             "        </fragOrders>"
                             "        <request>"
                             "            <transport>transportation mission name</transport>"
                             "            <embarkment>embarkment mission name</embarkment>"
                             "            <disembarkment>disembarkment mission name</disembarkment>"
                             "        </request>"
                             "    </missions>"
                             "    <reports>"
                             "        <mission-complete>1338</mission-complete>"
                             "    </reports>"
                             "</configuration>" );
    xis >> xml::start( "configuration" );
    tools::MessageController< sword::SimToClient_Content > messageController;
    MockMissionResolver resolver;
    MockCallsignResolver callsignResolver;
    MockSubordinates subordinates;
    dispatcher::MockSimulationPublisher publisher;
    MockContextFactory factory;
    MockInteractionSender< interactions::NetnRequestConvoy > requestSender;
    MockInteractionSender< interactions::NetnAcceptOffer > acceptSender;
    MockInteractionSender< interactions::NetnRejectOfferConvoy > rejectSender;
    MockInteractionSender< interactions::NetnReadyToReceiveService > readySender;
    MockInteractionSender< interactions::NetnServiceReceived > receivedSender;
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "transportation mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveUnit ).once().with( "transportation mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "embarkment mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveUnit ).once().with( "embarkment mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "disembarkment mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveUnit ).once().with( "disembarkment mission name" ).returns( 42 );
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "pause" ).returns( 43 );
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "resume" ).returns( 44 );
    MOCK_EXPECT( resolver, ResolveAutomat ).once().with( "cancel" ).returns( 45 );
    TransportationRequester requester( xis, resolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis( "<configuration>"
                   "    <missions>"
                   "        <fragOrders>"
                   "            <pause>pause</pause>"
                   "            <resume>resume</resume>"
                   "            <cancel>cancel</cancel>"
                   "        </fragOrders>"
                   "        <request>"
                   "            <transport>transport</transport>"
                   "            <embarkment>embarkment</embarkment>"
                   "            <disembarkment>disembarkment</disembarkment>"
                   "        </request>"
                   "    </missions>"
                   "    <reports>"
                   "        <mission-complete>1338</mission-complete>"
                   "    </reports>"
                   "</configuration>" )
            , transportAutomatId( 42 )
            , transportUnitId   ( 43 )
            , embarkmentAutomatId( 52 )
            , embarkmentUnitId   ( 53 )
            , disembarkmentAutomatId( 54 )
            , disembarkmentUnitId   ( 55 )
            , pauseId           ( 44 )
            , resumeId          ( 45 )
            , cancelId          ( 46 )
            , automatId         ( 47 )
            , unitId            ( 48 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "transport" ).returns( transportAutomatId );
            MOCK_EXPECT( missionResolver, ResolveUnit ).once().with( "transport" ).returns( transportUnitId );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "embarkment" ).returns( embarkmentAutomatId );
            MOCK_EXPECT( missionResolver, ResolveUnit ).once().with( "embarkment" ).returns( embarkmentUnitId );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "disembarkment" ).returns( disembarkmentAutomatId );
            MOCK_EXPECT( missionResolver, ResolveUnit ).once().with( "disembarkment" ).returns( disembarkmentUnitId );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "pause" ).returns( pauseId );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "resume" ).returns( resumeId );
            MOCK_EXPECT( missionResolver, ResolveAutomat ).once().with( "cancel" ).returns( cancelId );
        }
        sword::SimToClient_Content MakeTransportationMessage( unsigned int missionType )
        {
            sword::SimToClient_Content message;
            sword::AutomatOrder* automatOrder = message.mutable_automat_order();
            automatOrder->mutable_type()->set_id( missionType );
            return message;
        }
        xml::xistringstream xis;
        unsigned int transportAutomatId;
        unsigned int transportUnitId;
        unsigned int embarkmentAutomatId;
        unsigned int embarkmentUnitId;
        unsigned int disembarkmentAutomatId;
        unsigned int disembarkmentUnitId;
        unsigned int pauseId;
        unsigned int resumeId;
        unsigned int cancelId;
        unsigned long automatId;
        unsigned long unitId;
        MockMissionResolver missionResolver;
        dispatcher::MockSimulationPublisher publisher;
        tools::MessageController< sword::SimToClient_Content > messageController;
        MockCallsignResolver callsignResolver;
        MockSubordinates subordinates;
        MockContextFactory factory;
        MockInteractionSender< interactions::NetnRequestConvoy > requestSender;
        MockInteractionSender< interactions::NetnAcceptOffer > acceptSender;
        MockInteractionSender< interactions::NetnRejectOfferConvoy > rejectSender;
        MockInteractionSender< interactions::NetnReadyToReceiveService > readySender;
        MockInteractionSender< interactions::NetnServiceReceived > receivedSender;
    };
    bool CheckTransportedUnits( const TransportedUnits_ABC& visitable, const std::string& subordinateCallsign, const std::string& subordinateNetnUniqueId )
    {
        MockTransportedUnitsVisitor visitor;
        MOCK_EXPECT( visitor, Notify ).once().with( subordinateCallsign, subordinateNetnUniqueId );
        visitable.Apply( visitor );
        return true;
    }
    void AddLocation( sword::MissionParameters& parameters, const geometry::Point2d& point )
    {
        sword::Location* location = parameters.add_elem()->add_value()->mutable_location();
        location->set_type( sword::Location::point );
        sword::CoordLatLong* latLong = location->mutable_coordinates()->add_elem();
        latLong->set_latitude( point.X() );
        latLong->set_longitude( point.Y() );
    }
    void AddPoint( sword::MissionParameters& parameters, const geometry::Point2d& point )
    {
        sword::Location* location = parameters.add_elem()->add_value()->mutable_point()->mutable_location();
        location->set_type( sword::Location::point );
        sword::CoordLatLong* latLong = location->mutable_coordinates()->add_elem();
        latLong->set_latitude( point.X() );
        latLong->set_longitude( point.Y() );
    }
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_listens_to_automat_transportation_mission_and_notifies_listener_and_pauses_mission, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const geometry::Point2d embarkingPoint;
    const geometry::Point2d debarkingPoint;
    const long long embarkingTime = 1;
    const long long debarkingTime = 2;
    const std::string subordinateCallsign = "subordinate callsign";
    const std::string subordinateNetnUniqueId = "143";
    const std::string transportingUnitCallsign = "transporting callsign";
    sword::SimToClient_Content message = MakeTransportationMessage( transportAutomatId );
    sword::AutomatOrder* automatOrder = message.mutable_automat_order();
    automatOrder->mutable_tasker()->set_id( automatId );
    sword::MissionParameters* parameters = automatOrder->mutable_parameters();
    parameters->add_elem();// danger direction
    parameters->add_elem();// phase lines
    parameters->add_elem();// limit 1
    parameters->add_elem();// limit 2
    AddLocation( *parameters, embarkingPoint ); // embarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000001" ); // embarking time
    AddLocation( *parameters, debarkingPoint ); // debarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000002" ); // debarking time
    interactions::NetnRequestConvoy convoy;
    sword::ClientToSim pauseMessage;
    MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateCallsign, subordinateNetnUniqueId ) );
    MOCK_EXPECT( requestSender, Send ).once().with( mock::retrieve( convoy ) );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( pauseMessage ) );
    messageController.Dispatch( message );
    BOOST_CHECK( pauseMessage.message().has_frag_order() );
    BOOST_CHECK_EQUAL( pauseMessage.message().frag_order().type().id(), pauseId );
    const int convoyServiceType = 4;
    const unsigned int noTimeout = 0;
    const int noDetail = 0;
    BOOST_CHECK_EQUAL( convoy.serviceId.eventCount, 1337 );
    BOOST_CHECK_EQUAL( convoy.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.consumer.str(), "SWORD" );
    BOOST_CHECK( convoy.provider.str().empty() );
    BOOST_CHECK_EQUAL( convoy.serviceType, convoyServiceType );
    BOOST_CHECK_EQUAL( convoy.requestTimeOut, noTimeout );
    BOOST_CHECK_EQUAL( convoy.transportData.convoyType, NetnTransportStruct::E_Transport );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.appointment.dateTime, embarkingTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Latitude(), embarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Longitude(), embarkingPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.finalAppointment.dateTime, debarkingTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Latitude(), debarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Longitude(), debarkingPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].callsign.str(), subordinateCallsign );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].uniqueId.str(), subordinateNetnUniqueId );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].objectFeature.featureLevel, noDetail );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_listens_to_automat_embarkment_mission_and_notifies_listener_and_pauses_mission, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const geometry::Point2d embarkingPoint;
    const long long embarkingTime = 1;
    const std::string subordinateCallsign = "subordinate callsign";
    const std::string subordinateNetnUniqueId = "143";
    const std::string transportingUnitCallsign = "transporting callsign";
    sword::SimToClient_Content message = MakeTransportationMessage( embarkmentAutomatId );
    sword::AutomatOrder* automatOrder = message.mutable_automat_order();
    automatOrder->mutable_tasker()->set_id( automatId );
    sword::MissionParameters* parameters = automatOrder->mutable_parameters();
    parameters->add_elem();// danger direction
    parameters->add_elem();// phase lines
    parameters->add_elem();// limit 1
    parameters->add_elem();// limit 2
    AddLocation( *parameters, embarkingPoint ); // embarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000001" ); // embarking time
    interactions::NetnRequestConvoy convoy;
    sword::ClientToSim pauseMessage;
    MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateCallsign, subordinateNetnUniqueId ) );
    MOCK_EXPECT( requestSender, Send ).once().with( mock::retrieve( convoy ) );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( pauseMessage ) );
    messageController.Dispatch( message );
    BOOST_CHECK( pauseMessage.message().has_frag_order() );
    BOOST_CHECK_EQUAL( pauseMessage.message().frag_order().type().id(), pauseId );
    const int convoyServiceType = 4;
    const unsigned int noTimeout = 0;
    const int noDetail = 0;
    BOOST_CHECK_EQUAL( convoy.serviceId.eventCount, 1337 );
    BOOST_CHECK_EQUAL( convoy.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.consumer.str(), "SWORD" );
    BOOST_CHECK( convoy.provider.str().empty() );
    BOOST_CHECK_EQUAL( convoy.serviceType, convoyServiceType );
    BOOST_CHECK_EQUAL( convoy.requestTimeOut, noTimeout );
    BOOST_CHECK_EQUAL( convoy.transportData.convoyType, NetnTransportStruct::E_Embarkment );
    BOOST_CHECK_EQUAL( convoy.transportData.dataEmbarkment.appointment.dateTime, embarkingTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataEmbarkment.appointment.location.Latitude(), embarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataEmbarkment.appointment.location.Longitude(), embarkingPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataEmbarkment.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( convoy.transportData.dataEmbarkment.objectToManage[ 0 ].callsign.str(), subordinateCallsign );
    BOOST_CHECK_EQUAL( convoy.transportData.dataEmbarkment.objectToManage[ 0 ].uniqueId.str(), subordinateNetnUniqueId );
    BOOST_CHECK_EQUAL( convoy.transportData.dataEmbarkment.objectToManage[ 0 ].objectFeature.featureLevel, noDetail );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_listens_to_automat_disembarkment_mission_and_notifies_listener_and_pauses_mission, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const geometry::Point2d disembarkingPoint;
    const long long disembarkingTime = 1;
    const std::string subordinateCallsign = "subordinate callsign";
    const std::string subordinateNetnUniqueId = "143";
    const std::string transportingUnitCallsign = "transporting callsign";
    sword::SimToClient_Content message = MakeTransportationMessage( disembarkmentAutomatId );
    sword::AutomatOrder* automatOrder = message.mutable_automat_order();
    automatOrder->mutable_tasker()->set_id( automatId );
    sword::MissionParameters* parameters = automatOrder->mutable_parameters();
    parameters->add_elem();// danger direction
    parameters->add_elem();// phase lines
    parameters->add_elem();// limit 1
    parameters->add_elem();// limit 2
    AddLocation( *parameters, disembarkingPoint ); // disembarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000001" ); // disembarking time
    interactions::NetnRequestConvoy convoy;
    sword::ClientToSim pauseMessage;
    MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateCallsign, subordinateNetnUniqueId ) );
    MOCK_EXPECT( requestSender, Send ).once().with( mock::retrieve( convoy ) );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( pauseMessage ) );
    messageController.Dispatch( message );
    BOOST_CHECK( pauseMessage.message().has_frag_order() );
    BOOST_CHECK_EQUAL( pauseMessage.message().frag_order().type().id(), pauseId );
    const int convoyServiceType = 4;
    const unsigned int noTimeout = 0;
    const int noDetail = 0;
    BOOST_CHECK_EQUAL( convoy.serviceId.eventCount, 1337 );
    BOOST_CHECK_EQUAL( convoy.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.consumer.str(), "SWORD" );
    BOOST_CHECK( convoy.provider.str().empty() );
    BOOST_CHECK_EQUAL( convoy.serviceType, convoyServiceType );
    BOOST_CHECK_EQUAL( convoy.requestTimeOut, noTimeout );
    BOOST_CHECK_EQUAL( convoy.transportData.convoyType, NetnTransportStruct::E_Disembarkment );
    BOOST_CHECK_EQUAL( convoy.transportData.dataDisembarkment.appointment.dateTime, disembarkingTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataDisembarkment.appointment.location.Latitude(), disembarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataDisembarkment.appointment.location.Longitude(), disembarkingPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataDisembarkment.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( convoy.transportData.dataDisembarkment.objectToManage[ 0 ].callsign.str(), subordinateCallsign );
    BOOST_CHECK_EQUAL( convoy.transportData.dataDisembarkment.objectToManage[ 0 ].uniqueId.str(), subordinateNetnUniqueId );
    BOOST_CHECK_EQUAL( convoy.transportData.dataDisembarkment.objectToManage[ 0 ].objectFeature.featureLevel, noDetail );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_listens_to_unit_transportation_mission_and_notifies_listener_and_pauses_mission, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const geometry::Point2d embarkingPoint( 1., 2. );
    const geometry::Point2d debarkingPoint( -1., -2. );
    const std::string unitCallsign = "callsign";
    const std::string unitNetnUniqueId = "143";
    sword::SimToClient_Content message;
    sword::UnitOrder* unitOrder = message.mutable_unit_order();
    unitOrder->mutable_type()->set_id( transportUnitId );
    unitOrder->mutable_tasker()->set_id( unitId );
    sword::MissionParameters* parameters = unitOrder->mutable_parameters();
    parameters->add_elem();// danger direction
    parameters->add_elem();// phase lines
    parameters->add_elem();// limit 1
    parameters->add_elem();// limit 2
    AddPoint( *parameters, embarkingPoint ); // embarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000001" ); // embarking time
    AddPoint( *parameters, debarkingPoint ); // debarking point
    parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000002" ); // debarking time
    interactions::NetnRequestConvoy convoy;
    sword::ClientToSim pauseMessage;
    MOCK_EXPECT( requestSender, Send ).once().with( mock::retrieve( convoy ) );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( pauseMessage ) );
    MOCK_EXPECT( callsignResolver, ResolveUniqueId ).once().with( unitId ).returns( unitNetnUniqueId );
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( unitId ).returns( unitCallsign );
    messageController.Dispatch( message );
    BOOST_CHECK( pauseMessage.message().has_frag_order() );
    BOOST_CHECK_EQUAL( pauseMessage.message().frag_order().type().id(), pauseId );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Latitude(), embarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Longitude(), embarkingPoint.Y(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Latitude(), debarkingPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Longitude(), debarkingPoint.Y(), 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( transporation_controller_does_nothing_if_mission_is_not_transportation, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const unsigned int unknownMission = transportAutomatId + 100;
    messageController.Dispatch( MakeTransportationMessage( unknownMission ) );
}

namespace
{
    class RequestedFixture : public Fixture
    {
    public:
        RequestedFixture()
            : requester                   ( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender )
            , context                     ( 1337 )
            , missionCompleteId           ( 1338u )
            , embarkingPoint              ( 1., 2. )
            , embarkingTime               ( 1 )
            , debarkingTime               ( 2 )
            , automatId                   ( 42 )
            , subordinateCallsign         ( "subordinate callsign" )
            , subordinateNetnUniqueId     ( "143" )
            , transportingUnitSimulationId( 43 )
            , transportingUnitCallsign    ( "transporting callsign" )
            , transportingUnitUniqueId    ( "77777" )
        {
            listOfTransporters.list.push_back( NetnObjectDefinitionStruct( transportingUnitCallsign, transportingUnitUniqueId, NetnObjectFeatureStruct() ) );
            sword::SimToClient_Content message = MakeTransportationMessage( transportAutomatId );
            sword::AutomatOrder* automatOrder = message.mutable_automat_order();
            automatOrder->mutable_tasker()->set_id( automatId );
            sword::MissionParameters* parameters = automatOrder->mutable_parameters();
            parameters->add_elem();// danger direction
            parameters->add_elem();// phase lines
            parameters->add_elem();// limit 1
            parameters->add_elem();// limit 2
            AddLocation( *parameters, embarkingPoint ); // embarking point
            parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000001" ); // embarking time
            AddLocation( *parameters, debarkingPoint ); // debarking point
            parameters->add_elem()->add_value()->mutable_datetime()->set_data( "19700101T000002" ); // debarking time
            MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateCallsign, subordinateNetnUniqueId ) );
            MOCK_EXPECT( requestSender, Send ).once();
            MOCK_EXPECT( factory, Create ).once().returns( context );
            MOCK_EXPECT( publisher, SendClientToSim ).once();
            messageController.Dispatch( message );
        }
        sword::SimToClient_Content MakeReportMessage( unsigned int automatId, unsigned int reportType )
        {
            sword::SimToClient_Content message;
            sword::Report* report = message.mutable_report();
            report->mutable_source()->mutable_automat()->set_id( automatId );
            report->mutable_type()->set_id( reportType );
            return message;
        }
        void FillService( interactions::NetnService& service, const std::string& provider, int context )
        {
            service.consumer = UnicodeString( "SWORD" );
            service.provider = UnicodeString( provider );
            service.serviceId = NetnEventIdentifier( context, "SWORD" );
            service.serviceType = 4;
        }
        interactions::NetnOfferConvoy MakeOffer( unsigned int offerType, const std::string& provider )
        {
            interactions::NetnOfferConvoy offer;
            FillService( offer, provider, 1337 );
            offer.isOffering = true;
            offer.requestTimeOut = 0;
            offer.listOfTransporters = listOfTransporters;
            offer.offerType = offerType;
            return offer;
        }
        TransportationRequester requester;
        const unsigned int context;
        const unsigned int missionCompleteId;
        const geometry::Point2d embarkingPoint;
        const geometry::Point2d debarkingPoint;
        const long long embarkingTime;
        const long long debarkingTime;
        const unsigned long automatId;
        const std::string subordinateCallsign;
        const std::string subordinateNetnUniqueId;
        const unsigned long transportingUnitSimulationId;
        const std::string transportingUnitCallsign;
        const std::string transportingUnitUniqueId;
        interactions::ListOfUnits listOfTransporters;
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_accepts_offer_if_offer_is_same_as_request_and_notifies_listener_and_resume_mission, RequestedFixture )
{
    MOCK_EXPECT( acceptSender, Send ).once();
    interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
    sword::ClientToSim resumeMessage;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( resumeMessage ) );
    requester.Receive( offer );
    BOOST_CHECK( resumeMessage.message().has_frag_order() );
    BOOST_CHECK_EQUAL( resumeMessage.message().frag_order().type().id(), resumeId );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_rejects_offer_if_offer_is_partial_and_notifies_listener, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 2, "provider" );
    interactions::NetnRejectOfferConvoy reject;
    MOCK_EXPECT( rejectSender, Send ).once().with( mock::retrieve( reject ) );
    requester.Receive( offer );
    BOOST_CHECK_EQUAL( reject.reason, "Offering only partial offer" );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_reject_all_good_offers_if_already_accepted, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
    MOCK_EXPECT( acceptSender, Send ).once();
    MOCK_EXPECT( publisher, SendClientToSim ).once();
    requester.Receive( offer );
    mock::verify();
    interactions::NetnOfferConvoy secondOffer = MakeOffer( 1, "provider2" );
    interactions::NetnRejectOfferConvoy reject;
    MOCK_EXPECT( rejectSender, Send ).once().with( mock::retrieve( reject ) );
    requester.Receive( secondOffer );
    BOOST_CHECK_EQUAL( reject.reason, "An other offer has already been accepted" );
    BOOST_CHECK_EQUAL( reject.provider.str(), "provider2" );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_does_nothing_when_report_mission_complete_but_offer_rejected, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 2, "provider" );
    MOCK_EXPECT( rejectSender, Send ).once();
    requester.Receive( offer );
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_does_nothing_if_service_id_is_not_known, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 2, "provider" );
    offer.serviceId.eventCount++;
    requester.Receive( offer );
}

namespace
{
    class AcceptedFixture : public RequestedFixture
    {
    public:
        AcceptedFixture()
        {
            MOCK_EXPECT( acceptSender, Send ).once();
            MOCK_EXPECT( publisher, SendClientToSim ).once();
            interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
            requester.Receive( offer );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_notifies_ready_to_receive_service_when_receiving_mission_complete_report, AcceptedFixture )
{
    MOCK_EXPECT( readySender, Send ).once();
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_does_nothing_when_report_is_not_mission_complete_report, AcceptedFixture )
{
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId + 1 );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_does_nothing_when_report_is_not_for_transported_automat, AcceptedFixture )
{
    sword::SimToClient_Content message = MakeReportMessage( automatId + 1, missionCompleteId );
    messageController.Dispatch( message );
}

namespace
{
    class ReadyAndStartedFixture : public AcceptedFixture
    {
    public:
        ReadyAndStartedFixture()
        {
            MOCK_EXPECT( readySender, Send ).once();
            sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
            messageController.Dispatch( message );
            interactions::NetnServiceStarted started;
            FillService( started, "provider", 1337 );
            requester.Receive( started );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_sends_load_unit_magic_action_for_every_embarked_unit, ReadyAndStartedFixture )
{
    const unsigned int vesselId = 888;
    const unsigned int surbordinateId = 999;
    MockTransportedUnits units;
    sword::ClientToSim message;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportingUnitUniqueId ).returns( vesselId );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
    interactions::NetnConvoyEmbarkmentStatus status;
    FillService( status, "provider", 1337 );
    status.transportUnitIdentifier = UnicodeString( transportingUnitCallsign );
    status.listOfObjectEmbarked.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
    requester.Receive( status );
    mock::verify();
    BOOST_CHECK( message.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = message.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), vesselId );
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::load_unit );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    BOOST_CHECK( action.parameters().elem( 0 ).value( 0 ).has_agent() );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).agent().id(), surbordinateId );
}

namespace
{
    class EmbarkedFixture : public ReadyAndStartedFixture
    {
    public:
        EmbarkedFixture()
            : vesselId      ( 888 )
            , surbordinateId( 999 )
        {
            MOCK_EXPECT( publisher, SendClientToSim ).once();
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportingUnitUniqueId ).returns( vesselId );
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
            interactions::NetnConvoyEmbarkmentStatus status;
            FillService( status, "provider", 1337 );
            status.transportUnitIdentifier = UnicodeString( transportingUnitCallsign );
            status.listOfObjectEmbarked.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
            requester.Receive( status );
        }
        const unsigned int vesselId;
        const unsigned int surbordinateId;
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_sends_destroy_all_magic_action_for_every_destroyed_entity, EmbarkedFixture )
{
    interactions::NetnConvoyDestroyedEntities destroyed;
    FillService( destroyed, "provider", 1337 );
    destroyed.listOfEmbarkedObjectDestroyed.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
    sword::ClientToSim message;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
    requester.Receive( destroyed );
    mock::verify();
    BOOST_CHECK( message.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = message.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), surbordinateId );
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::destroy_all );
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_sends_unload_unit_magic_action_for_every_disembarked_unit_and_teleport_to_disembarking_point, EmbarkedFixture )
{
    interactions::NetnConvoyDisembarkmentStatus status;
    FillService( status, "provider", 1337 );
    status.transportUnitIdentifier = UnicodeString( transportingUnitCallsign );
    status.listOfObjectDisembarked.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
    sword::ClientToSim message;
    sword::ClientToSim teleport;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( teleport ) );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportingUnitUniqueId ).returns( vesselId );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
    requester.Receive( status );
    mock::verify();
    BOOST_CHECK( message.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = message.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), vesselId );
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::unload_unit );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    BOOST_CHECK( action.parameters().elem( 0 ).value( 0 ).has_agent() );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).agent().id(), surbordinateId );
    BOOST_CHECK( teleport.message().has_unit_magic_action() );
    BOOST_CHECK_EQUAL( teleport.message().unit_magic_action().type(), sword::UnitMagicAction::move_to );
}

namespace
{
    class DebarkedFixture : public EmbarkedFixture
    {
    public:
        DebarkedFixture()
        {
            interactions::NetnConvoyDisembarkmentStatus status;
            FillService( status, "provider", 1337 );
            status.transportUnitIdentifier = UnicodeString( transportingUnitCallsign );
            status.listOfObjectDisembarked.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
            MOCK_EXPECT( publisher, SendClientToSim ).exactly( 2 );
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportingUnitUniqueId ).returns( vesselId );
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
            requester.Receive( status );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_requester_notifies_service_received_when_complete, DebarkedFixture )
{
    MOCK_EXPECT( receivedSender, Send ).once();
    interactions::NetnServiceComplete complete;
    FillService( complete, "provider", 1337 );
    requester.Receive( complete );
}
