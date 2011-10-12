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
#include "hla_plugin/TransportedUnits_ABC.h"
#include "MockMissionResolver.h"
#include "MockTransportationListener.h"
#include "MockCallsignResolver.h"
#include "MockTransportedUnitsVisitor.h"
#include "MockSubordinates.h"
#include "MockContextFactory.h"
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
                             "    <reports>"
                             "        <mission-complete>1338</mission-complete>"
                             "    </reports>"
                             "</configuration>" );
    xis >> xml::start( "configuration" );
    tools::MessageController< sword::SimToClient_Content > messageController;
    MockMissionResolver resolver;
    MockCallsignResolver callsignResolver;
    MockSubordinates subordinates;
    MockContextFactory factory;
    MOCK_EXPECT( resolver, Resolve ).once().with( "transportation mission name" ).returns( 42 );
    TransportationController controller( xis, resolver, messageController, callsignResolver, subordinates, factory );
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
                   "    <reports>"
                   "        <mission-complete>1338</mission-complete>"
                   "    </reports>"
                   "</configuration>" )
            , transportId( 42 )
            , automatId  ( 42 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( missionResolver, Resolve ).once().returns( transportId );
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
        MockTransportationListener listener;
        tools::MessageController< sword::SimToClient_Content > messageController;
        MockCallsignResolver callsignResolver;
        MockSubordinates subordinates;
        MockContextFactory factory;
    };
    bool CheckTransportedUnits( const TransportedUnits_ABC& visitable, const std::string& subordinateNetnUniqueId, const std::string& subordinateCallsign )
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
    TransportationController controller( xis, missionResolver, messageController, callsignResolver, subordinates, factory );
    controller.Register( listener );
    const geometry::Point2d embarkingPoint;
    const geometry::Point2d debarkingPoint;
    const long long embarkingTime = 1;
    const long long debarkingTime = 2;
    const std::string subordinateCallsign = "subordinate callsign";
    const std::string subordinateNetnUniqueId = "143";
    const unsigned long transportingUnitSimulationId = 43;
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
    parameters->add_elem()->add_value()->mutable_agent()->set_id( transportingUnitSimulationId );// transporting unit
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( transportingUnitSimulationId ).returns( transportingUnitCallsign );
    MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateNetnUniqueId, subordinateCallsign ) );
    MOCK_EXPECT( listener, ConvoyRequested ).once().with( transportingUnitCallsign, embarkingTime, embarkingPoint, debarkingTime, debarkingPoint, boost::bind( &CheckTransportedUnits, _1, boost::cref( subordinateCallsign ), boost::cref( subordinateNetnUniqueId ) ), 1337u );
    MOCK_EXPECT( factory, Create ).once().returns( 1337 );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transporation_controller_does_nothing_if_mission_is_not_transportation, Fixture )
{
    TransportationController controller( xis, missionResolver, messageController, callsignResolver, subordinates, factory );
    controller.Register( listener );
    const unsigned int unknownMission = transportId + 1;
    messageController.Dispatch( MakeTransportationMessage( unknownMission ) );
}

namespace
{
    class RequestedFixture : public Fixture
    {
    public:
        RequestedFixture()
            : controller                  ( xis, missionResolver, messageController, callsignResolver, subordinates, factory )
            , context                     ( 1337 )
            , missionCompleteId           ( 1338u )
            , embarkingTime               ( 1 )
            , debarkingTime               ( 2 )
            , automatId                   ( 42 )
            , subordinateCallsign         ( "subordinate callsign" )
            , subordinateNetnUniqueId     ( "143" )
            , transportingUnitSimulationId( 43 )
            , transportingUnitCallsign    ( "transporting callsign" )
        {
            controller.Register( listener );
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
            parameters->add_elem()->add_value()->mutable_agent()->set_id( transportingUnitSimulationId );// transporting unit
            MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( transportingUnitSimulationId ).returns( transportingUnitCallsign );
            MOCK_EXPECT( subordinates, Apply ).once().with( automatId, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _2, subordinateNetnUniqueId, subordinateCallsign ) );
            MOCK_EXPECT( listener, ConvoyRequested ).once().with( transportingUnitCallsign, embarkingTime, embarkingPoint, debarkingTime, debarkingPoint, boost::bind( &CheckTransportedUnits, _1, boost::cref( subordinateCallsign ), boost::cref( subordinateNetnUniqueId ) ), context );
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
        TransportationController controller;
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
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_accepts_offer_if_offer_is_same_as_request_and_notifies_listener, RequestedFixture )
{
    MOCK_EXPECT( listener, OfferAccepted ).once().with( context, "provider" );
    controller.OfferReceived( context, true, "provider" );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_rejects_offer_if_offer_is_partial_and_notifies_listener, RequestedFixture )
{
    MOCK_EXPECT( listener, OfferRejected ).once().with( context, "provider", "Not offering service or partial offer" );
    controller.OfferReceived( context, false, "provider" );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_reject_all_good_offers_if_already_accepted, RequestedFixture )
{
    MOCK_EXPECT( listener, OfferAccepted ).once().with( context, "provider" );
    controller.OfferReceived( context, true, "provider" );
    mock::verify();
    MOCK_EXPECT( listener, OfferRejected ).once().with( context, "provider2", "An other offer has already been accepted" );
    controller.OfferReceived( context, true, "provider2" );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_when_report_mission_complete_but_offer_rejected, RequestedFixture )
{
    MOCK_EXPECT( listener, OfferRejected ).once();
    controller.OfferReceived( context, false, "provider" );
    sword::SimToClient_Content message = MakeReportMessage( automatId, missionCompleteId );
    messageController.Dispatch( message );
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_does_nothing_if_service_id_is_not_known, RequestedFixture )
{
    controller.OfferReceived( context + 1, true, "provider" );
}

namespace
{
    class AcceptedFixture : public RequestedFixture
    {
    public:
        AcceptedFixture()
        {
            MOCK_EXPECT( listener, OfferAccepted ).once().with( context, "provider" );
            controller.OfferReceived( context, true, "provider" );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_controller_notifies_ready_to_receive_service_when_receiving_mission_complete_report, AcceptedFixture )
{
    MOCK_EXPECT( listener, ReadyToReceiveService ).once().with( context, "provider" );
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
