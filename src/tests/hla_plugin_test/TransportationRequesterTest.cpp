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

BOOST_AUTO_TEST_CASE( transportation_controller_reads_transportation_mission_name_from_xml )
{
    xml::xistringstream xis( "<configuration>"
                             "    <missions>"
                             "        <request>"
                             "            <transport>transportation mission name</transport>"
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
    MOCK_EXPECT( resolver, Resolve ).once().with( "transportation mission name" ).returns( 42 );
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
                   "        <request>"
                   "            <transport>transport</transport>"
                   "        </request>"
                   "    </missions>"
                   "    <reports>"
                   "        <mission-complete>1338</mission-complete>"
                   "    </reports>"
                   "</configuration>" )
            , transportId( 42 )
            , automatId  ( 42 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( missionResolver, Resolve ).once().with( "transport" ).returns( transportId );
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
        unsigned long automatId;
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
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_listens_to_transportation_mission_and_notifies_listener, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const geometry::Point2d embarkingPoint;
    const geometry::Point2d debarkingPoint;
    const long long embarkingTime = 1;
    const long long debarkingTime = 2;
    const std::string subordinateCallsign = "subordinate callsign";
    const std::string subordinateNetnUniqueId = "143";
    const std::string transportingUnitCallsign = "transporting callsign";
    sword::SimToClient_Content message = MakeTransportationMessage( transportId );
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
    MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateCallsign, subordinateNetnUniqueId ) );
    MOCK_EXPECT( requestSender, Send ).once().with( mock::retrieve( convoy ) );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    messageController.Dispatch( message );
    const int convoyServiceType = 4;
    const unsigned int noTimeout = 0;
    const int convoyTransportType = 0;
    const int noDetail = 0;
    BOOST_CHECK_EQUAL( convoy.serviceId.eventCount, 1337 );
    BOOST_CHECK_EQUAL( convoy.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.consumer.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.provider.str(), "Any carrier" );
    BOOST_CHECK_EQUAL( convoy.serviceType, convoyServiceType );
    BOOST_CHECK_EQUAL( convoy.requestTimeOut, noTimeout );
    BOOST_CHECK_EQUAL( convoy.transportData.convoyType, convoyTransportType );
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

BOOST_FIXTURE_TEST_CASE( transporation_controller_does_nothing_if_mission_is_not_transportation, Fixture )
{
    TransportationRequester requester( xis, missionResolver, messageController, callsignResolver, subordinates, factory, publisher, requestSender, acceptSender, rejectSender, readySender, receivedSender );
    const unsigned int unknownMission = transportId + 1;
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
            sword::SimToClient_Content message = MakeTransportationMessage( transportId );
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

BOOST_FIXTURE_TEST_CASE( transportation_controller_accepts_offer_if_offer_is_same_as_request_and_notifies_listener, RequestedFixture )
{
    MOCK_EXPECT( acceptSender, Send ).once();
    interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
    requester.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_rejects_offer_if_offer_is_partial_and_notifies_listener, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 2, "provider" );
    interactions::NetnRejectOfferConvoy reject;
    MOCK_EXPECT( rejectSender, Send ).once().with( mock::retrieve( reject ) );
    requester.Receive( offer );
    BOOST_CHECK_EQUAL( reject.reason, "Offering only partial offer" );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_reject_all_good_offers_if_already_accepted, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
    MOCK_EXPECT( acceptSender, Send ).once();
    requester.Receive( offer );
    mock::verify();
    interactions::NetnOfferConvoy secondOffer = MakeOffer( 1, "provider2" );
    interactions::NetnRejectOfferConvoy reject;
    MOCK_EXPECT( rejectSender, Send ).once().with( mock::retrieve( reject ) );
    requester.Receive( secondOffer );
    BOOST_CHECK_EQUAL( reject.reason, "An other offer has already been accepted" );
    BOOST_CHECK_EQUAL( reject.provider.str(), "provider2" );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_when_report_mission_complete_but_offer_rejected, RequestedFixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 2, "provider" );
    MOCK_EXPECT( rejectSender, Send ).once();
    requester.Receive( offer );
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_if_service_id_is_not_known, RequestedFixture )
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
            interactions::NetnOfferConvoy offer = MakeOffer( 1, "provider" );
            requester.Receive( offer );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_notifies_ready_to_receive_service_when_receiving_mission_complete_report, AcceptedFixture )
{
    MOCK_EXPECT( readySender, Send ).once();
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_when_report_is_not_mission_complete_report, AcceptedFixture )
{
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId + 1 );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_when_report_is_not_for_transported_automat, AcceptedFixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_controller_sends_load_unit_magic_action_for_every_embarked_unit, ReadyAndStartedFixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_controller_sends_unload_unit_magic_action_for_every_disembarked_unit, EmbarkedFixture )
{
    interactions::NetnConvoyDisembarkmentStatus status;
    FillService( status, "provider", 1337 );
    status.transportUnitIdentifier = UnicodeString( transportingUnitCallsign );
    status.listOfObjectDisembarked.list.push_back( NetnObjectDefinitionStruct( subordinateCallsign, subordinateNetnUniqueId, NetnObjectFeatureStruct() ) );
    sword::ClientToSim message;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
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
            MOCK_EXPECT( publisher, SendClientToSim ).once();
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( transportingUnitUniqueId ).returns( vesselId );
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( subordinateNetnUniqueId ).returns( surbordinateId );
            requester.Receive( status );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_notifies_service_received_when_complete, DebarkedFixture )
{
    MOCK_EXPECT( receivedSender, Send ).once();
    interactions::NetnServiceComplete complete;
    FillService( complete, "provider", 1337 );
    requester.Receive( complete );
}
