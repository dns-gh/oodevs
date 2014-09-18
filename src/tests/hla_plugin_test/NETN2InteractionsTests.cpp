// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Interactions.h"
#include "SerializationFixture.h"
#include "hla_plugin/NETNv2_InteractionBuilder.h"
#include "MockLogger.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockInteractionNotification.h"
#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>
#include <utility>

using namespace plugins::hla;

/* Check serialization against data recorded in interop tests with Orque

Interaction: 'SCP_Service.SCP_RequestService.RequestTransport':
Consumer = [00000005 00530057 004F0052 0044]
ServiceID = [00000050 00000005 00530057 004F0052 0044]
ServiceType = [04]
RequestTimeOut = [00000000 00000000]
TransportData = [00000000 00000000 00000001 A8E0E382 1B8347BA 846A8452 54932C70 00000000 00000000 53D916F5 414CA0AB 2150B9C9 411E98CE 3231B90D 41538415 3D82C448 00000000 53D916F5 414CA9FE 174F232F 411D309A 8523F015 415382D3 336E5AD1]
Provider = [00000000]
Logical Time: [40A69400 00000000]
User-supplied Tag: []
Producing Federate: SWORD

Interaction: 'SCP_Service.SCP_RequestService.RequestTransport':
Consumer = SWORD
ServiceID = {80, SWORD}
ServiceType = Transport
RequestTimeOut = 0
TransportData = (Transport: DataTransport = {[[0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70]], {1406736117, {X:3752278.2602760536, Y:501299.54901780264, Z:5115988.961106367 (53.68395 N, 7.60958 E, 0.00)}}, {1406736117, {X:3757052.182102583, Y:478246.6300199044, Z:5114700.8036105195 (53.66441 N, 7.25434 E, 0.00)}}})
Provider =
Logical Time: [40A69400 00000000]
User-supplied Tag: []
Producing Federate: SWORD

--------------------------------------------------------------------------

Interaction: 'SCP_Service.SCP_OfferService.OfferTransport':
IsOffering = [00000001]
Consumer = [00000005 00530057 004F0052 0044]
ServiceID = [00000050 00000005 00530057 004F0052 0044]
ServiceType = [04]
RequestTimeOut = [00000000 5412EE50]
OfferType = [00000001]
TransportData = [00000000 00000000 00000001 A8E0E382 1B8347BA 846A8452 54932C70 00000000 00000000 5412D230 414CA0AB 281DF3C1 411E98CE 77092F0A 41538415 3A9CB1F4 00000000 54130A70 414CA9FE 082E5B8A 411D309A C9A84154 415382D3 388CCF9E]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Transporters = [00000001 E41A7728 C2034B8A 9C36761E CCF5C828]
Producing Federate: ORQUE

Interaction: 'SCP_Service.SCP_OfferService.OfferTransport':
IsOffering = true
Consumer = SWORD
ServiceID = {80, SWORD}
ServiceType = Transport
RequestTimeOut = 1410526800
OfferType = RequestAcknowledgePositive
TransportData = (Transport: DataTransport = {[[0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70]], {1410519600, {X:3752278.3134140675, Y:501299.6162459707, Z:5115988.915813912 (53.68395 N, 7.60958 E, 0.00)}}, {1410534000, {X:3757052.063914721, Y:478246.6969309051, Z:5114700.883594422 (53.66441 N, 7.25434 E, 0.00)}}})
Provider = ID_SIMU_001
Transporters = [[0xE4, 0x1A, 0x77, 0x28, 0xC2, 0x03, 0x4B, 0x8A, 0x9C, 0x36, 0x76, 0x1E, 0xCC, 0xF5, 0xC8, 0x28]]
Producing Federate: ORQUE

-----------------------------------------------------------------------------

Interaction: 'SCP_Service.SCP_AcceptOffer':
ServiceID = [00000050 00000005 00530057 004F0052 0044]
Consumer = [00000005 00530057 004F0052 0044]
ServiceType = [04]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Logical Time: [40BA9A00 00000000]
User-supplied Tag: []
Producing Federate: SWORD

Interaction: 'SCP_Service.SCP_AcceptOffer':
ServiceID = {80, SWORD}
Consumer = SWORD
ServiceType = Transport
Provider = ID_SIMU_001
Logical Time: [40BA9A00 00000000]
User-supplied Tag: []
Producing Federate: SWORD

----------------------------------------------------------------------------------


Interaction: 'SCP_Service.SCP_ReadyToReceiveService':
ServiceID = [00000050 00000005 00530057 004F0052 0044]
Consumer = [00000005 00530057 004F0052 0044]
ServiceType = [04]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Logical Time: [40BD8800 00000000]
User-supplied Tag: []
Producing Federate: SWORD


Interaction: 'SCP_Service.SCP_ReadyToReceiveService':
ServiceID = {80, SWORD}
Consumer = SWORD
ServiceType = Transport
Provider = ID_SIMU_001
Logical Time: [40BD8800 00000000]
User-supplied Tag: []
Producing Federate: SWORD

----------------------------------------------------------------------------------

Interaction: 'SCP_Service.SCP_ServiceStarted':
ServiceID = [00000050 00000005 00530057 004F0052 0044]
Consumer = [00000005 00530057 004F0052 0044]
ServiceType = [04]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Producing Federate: ORQUE

Interaction: 'SCP_Service.SCP_ServiceStarted':
ServiceID = {80, SWORD}
Consumer = SWORD
ServiceType = Transport
Provider = ID_SIMU_001
Producing Federate: ORQUE

----------------------------------------------------------------------------------------

Interaction: 'SCP_Service.TransportEmbarkmentStatus':
Consumer = [00000005 00530057 004F0052 0044]
ServiceID = [00000050 00000005 00530057 004F0052 0044]
ServiceType = [04]
EmbarkedObjects = [00000001 A8E0E382 1B8347BA 846A8452 54932C70]
TransportUnitIdentifier = [E41A7728 C2034B8A 9C36761E CCF5C828]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Producing Federate: ORQUE

Interaction: 'SCP_Service.TransportEmbarkmentStatus':
Consumer = SWORD
ServiceID = {80, SWORD}
ServiceType = Transport
EmbarkedObjects = [[0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70]]
TransportUnitIdentifier = [0xE4, 0x1A, 0x77, 0x28, 0xC2, 0x03, 0x4B, 0x8A, 0x9C, 0x36, 0x76, 0x1E, 0xCC, 0xF5, 0xC8, 0x28]
Provider = ID_SIMU_001
Producing Federate: ORQUE

-----------------------------------------------------------------------------------


Interaction: 'SCP_Service.TransportDisembarkmentStatus':
Consumer = [00000005 00530057 004F0052 0044]
ServiceID = [00000050 00000005 00530057 004F0052 0044]
ServiceType = [04]
TransportUnitIdentifier = [E41A7728 C2034B8A 9C36761E CCF5C828]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
DisembarkedObjects = [00000001 A8E0E382 1B8347BA 846A8452 54932C70]
Producing Federate: ORQUE


Interaction: 'SCP_Service.TransportDisembarkmentStatus':
Consumer = SWORD
ServiceID = {80, SWORD}
ServiceType = Transport
TransportUnitIdentifier = [0xE4, 0x1A, 0x77, 0x28, 0xC2, 0x03, 0x4B, 0x8A, 0x9C, 0x36, 0x76, 0x1E, 0xCC, 0xF5, 0xC8, 0x28]
Provider = ID_SIMU_001
DisembarkedObjects = [[0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70]]
Producing Federate: ORQUE

----------------------------------------------------------------------------------------

Interaction: 'SCP_Service.SCP_ServiceComplete':
ServiceID = [00000050 00000005 00530057 004F0052 0044]
Consumer = [00000005 00530057 004F0052 0044]
ServiceType = [04]
Provider = [0000000B 00490044 005F0053 0049004D 0055005F 00300030 0031]
Producing Federate: ORQUE

Interaction: 'SCP_Service.SCP_ServiceComplete':
ServiceID = {80, SWORD}
Consumer = SWORD
ServiceType = Transport
Provider = ID_SIMU_001
Producing Federate: ORQUE

 */

namespace
{
    struct NetnV2Fixture : SerializationFixture
    {
        NetnV2Fixture()
            : builder( logger , federate )
            , handler( new hla::MockInteractionHandler )
            , handlerPtr( handler ) {}

        void CheckSerializedValue( const ::hla::ParameterIdentifier& parameterID, ::hla::T_SerializerPtr serializer, const std::vector< uint8_t > expected)
        {
            std::vector< uint8_t > actual(serializer->GetSize());
            serializer->CopyTo(actual.data());

            BOOST_CHECK_MESSAGE( actual == expected, "mismatch in " + parameterID.ToString() );

            serializedData.push_back( actual );
            boost::shared_ptr< ::hla::Deserializer_ABC > deserializer( new ::hla::Deserializer( serializedData.rbegin()->data(), serializedData.rbegin()->size() ) );
            serializedParameters.push_back( std::make_pair( parameterID, deserializer ) );
        }

        void CheckParameter( const std::string& paramName, const std::vector< uint8_t > expected, hla::MockInteractionHandler& handler)
        {
            std::vector< uint8_t > actual;
            MOCK_EXPECT( handler.Visit ).once().with( paramName, mock::any ).calls( boost::bind( &NetnV2Fixture::CheckSerializedValue, this, _1, _2, expected ) );
        }

        dispatcher::MockLogger logger;
        MockFederate federate;
        NETNv2_InteractionBuilder builder;
        hla::MockInteractionHandler* handler;
        std::auto_ptr< ::hla::InteractionHandler_ABC > handlerPtr;
        std::vector< std::vector< uint8_t> > serializedData;
        ::hla::Interaction_ABC::T_Parameters serializedParameters;
    };

}

#define CHECK_PARAMETER(label, varname, handler, ...) \
    static const uint8_t varname[] = __VA_ARGS__; \
    CheckParameter( label, std::vector< uint8_t >( std::begin( varname ), std::end( varname ) ), *handler )

BOOST_FIXTURE_TEST_CASE( netn2_request_transport, NetnV2Fixture )
{
    interactions::NetnRequestConvoy request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.serviceType = 4; // Transport
    request.requestTimeOut = 0;
    const unsigned char uuid[] = { 0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70 };
    rpr::WorldLocation from, to;
    from.Reset( 3752278.2602760536, 501299.54901780264, 5115988.961106367 );
    to.Reset( 3757052.182102583, 478246.6300199044, 5114700.8036105195 );
    request.transportData = NetnTransportStruct( NetnDataTStruct(
            std::vector< NetnObjectDefinitionStruct >( 1, NetnObjectDefinitionStruct( "callsign", std::vector< char >( (char*)std::begin( uuid ), (char*)std::end( uuid ) ), NetnObjectFeatureStruct())),
            NetnAppointmentStruct( 1406736117, from ),
            NetnAppointmentStruct( 1406736117, to ) ) );

    ::hla::MockInteractionNotification< interactions::NetnRequestConvoy > notification;
    ::hla::Interaction< interactions::NetnRequestConvoy > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_RequestService.RequestTransport", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x00 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    CHECK_PARAMETER( "RequestTimeOut", timeout, handler , { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
    CHECK_PARAMETER( "TransportData", transportData, handler , { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA8, 0xE0,
            0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x53, 0xD9, 0x16, 0xF5, 0x41, 0x4C, 0xA0, 0xAB, 0x21, 0x50, 0xB9, 0xC9, 0x41, 0x1E, 0x98, 0xCE, 0x32, 0x31, 0xB9, 0x0D, 0x41, 0x53,
            0x84, 0x15, 0x3D, 0x82, 0xC4, 0x48, 0x00, 0x00, 0x00, 0x00, 0x53, 0xD9, 0x16, 0xF5, 0x41, 0x4C, 0xA9, 0xFE, 0x17, 0x4F, 0x23, 0x2F,
            0x41, 0x1D, 0x30, 0x9A, 0x85, 0x23, 0xF0, 0x15, 0x41, 0x53, 0x82, 0xD3, 0x33, 0x6E, 0x5A, 0xD1 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnRequestConvoy received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(),request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.requestTimeOut, request.requestTimeOut );
    BOOST_CHECK_EQUAL( received.transportData.convoyType, request.transportData.convoyType );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.objectToManage.size(), request.transportData.dataTransport.objectToManage.size() );
    BOOST_CHECK_EQUAL_COLLECTIONS( received.transportData.dataTransport.objectToManage[0].uniqueId.begin(), received.transportData.dataTransport.objectToManage[0].uniqueId.end(),
            request.transportData.dataTransport.objectToManage[0].uniqueId.begin(), request.transportData.dataTransport.objectToManage[0].uniqueId.end() );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.appointment.dateTime, request.transportData.dataTransport.appointment.dateTime );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.appointment.location, request.transportData.dataTransport.appointment.location );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.finalAppointment.dateTime, request.transportData.dataTransport.finalAppointment.dateTime );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.finalAppointment.location, request.transportData.dataTransport.finalAppointment.location );
}

BOOST_FIXTURE_TEST_CASE( netn2_offert_transport, NetnV2Fixture )
{
    interactions::NetnOfferConvoy request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport
    request.isOffering = 1;
    request.requestTimeOut = 1410526800;
    const unsigned char uuid[] = { 0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70 };
    rpr::WorldLocation from, to;
    from.Reset( 3752278.3134140675, 501299.6162459707, 5115988.915813912 );
    to.Reset( 3757052.063914721, 478246.6969309051, 5114700.883594422 );
    request.transportData = NetnTransportStruct( NetnDataTStruct(
            std::vector< NetnObjectDefinitionStruct >( 1, NetnObjectDefinitionStruct( "callsign", std::vector< char >( (char*)std::begin( uuid ), (char*)std::end( uuid ) ), NetnObjectFeatureStruct() ) ),
            NetnAppointmentStruct( 1410519600, from ),
            NetnAppointmentStruct( 1410534000, to ) ) );
    request.offerType = 1; // RequestAcknowledgePositive
    const unsigned char transporterUuid[] = { 0xE4, 0x1A, 0x77, 0x28, 0xC2, 0x03, 0x4B, 0x8A, 0x9C, 0x36, 0x76, 0x1E, 0xCC, 0xF5, 0xC8, 0x28 };
    request.listOfTransporters.list = std::vector< NetnObjectDefinitionStruct >( 1, NetnObjectDefinitionStruct( "callsign", std::vector< char >( (char*)std::begin( transporterUuid ), (char*)std::end( transporterUuid ) ), NetnObjectFeatureStruct() ) );

    ::hla::MockInteractionNotification< interactions::NetnOfferConvoy > notification;
    ::hla::Interaction< interactions::NetnOfferConvoy > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_OfferService.OfferTransport", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    CHECK_PARAMETER( "IsOffering", isOffering, handler , { 0x00, 0x00, 0x00, 0x01 } );
    CHECK_PARAMETER( "RequestTimeOut", timeout, handler , { 0x00, 0x00, 0x00, 0x00, 0x54, 0x12, 0xEE, 0x50 } );
    CHECK_PARAMETER( "TransportData", transportData, handler , { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA8, 0xE0, 0xE3, 0x82,
            0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x12, 0xD2, 0x30, 0x41,
            0x4C, 0xA0, 0xAB, 0x28, 0x1D, 0xF3, 0xC1, 0x41, 0x1E, 0x98, 0xCE, 0x77, 0x09, 0x2F, 0x0A, 0x41, 0x53, 0x84, 0x15, 0x3A, 0x9C, 0xB1, 0xF4, 0x00, 0x00,
            0x00, 0x00, 0x54, 0x13, 0x0A, 0x70, 0x41, 0x4C, 0xA9, 0xFE, 0x08, 0x2E, 0x5B, 0x8A, 0x41, 0x1D, 0x30, 0x9A, 0xC9, 0xA8, 0x41, 0x54, 0x41, 0x53, 0x82,
            0xD3, 0x38, 0x8C, 0xCF, 0x9E } );
    CHECK_PARAMETER( "OfferType", offerType, handler , { 0x00, 0x00, 0x00, 0x01 } );
    CHECK_PARAMETER( "Transporters", transporters, handler , { 0x00, 0x00, 0x00, 0x01, 0xE4, 0x1A, 0x77, 0x28, 0xC2, 0x03, 0x4B, 0x8A, 0x9C, 0x36, 0x76, 0x1E,  0xCC, 0xF5, 0xC8, 0x28 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnOfferConvoy received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.isOffering, request.isOffering );
    BOOST_CHECK_EQUAL( received.requestTimeOut, request.requestTimeOut );
    BOOST_CHECK_EQUAL( received.transportData.convoyType, request.transportData.convoyType );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.objectToManage.size(), request.transportData.dataTransport.objectToManage.size() );
    BOOST_CHECK_EQUAL_COLLECTIONS( received.transportData.dataTransport.objectToManage[0].uniqueId.begin(), received.transportData.dataTransport.objectToManage[0].uniqueId.end(),
            request.transportData.dataTransport.objectToManage[0].uniqueId.begin(), request.transportData.dataTransport.objectToManage[0].uniqueId.end() );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.appointment.dateTime, request.transportData.dataTransport.appointment.dateTime );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.appointment.location, request.transportData.dataTransport.appointment.location );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.finalAppointment.dateTime, request.transportData.dataTransport.finalAppointment.dateTime );
    BOOST_CHECK_EQUAL( received.transportData.dataTransport.finalAppointment.location, request.transportData.dataTransport.finalAppointment.location );
    BOOST_CHECK_EQUAL( received.offerType, request.offerType );
    BOOST_CHECK_EQUAL( received.listOfTransporters.list.size(), request.listOfTransporters.list.size() );
    BOOST_CHECK_EQUAL_COLLECTIONS( received.listOfTransporters.list[0].uniqueId.begin(), received.listOfTransporters.list[0].uniqueId.end(),
            request.listOfTransporters.list[0].uniqueId.begin(), request.listOfTransporters.list[0].uniqueId.end() );
}

BOOST_FIXTURE_TEST_CASE( netn2_accept_offer, NetnV2Fixture )
{
    interactions::NetnAcceptOffer request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport

    ::hla::MockInteractionNotification< interactions::NetnAcceptOffer > notification;
    ::hla::Interaction< interactions::NetnAcceptOffer > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_AcceptOffer", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnAcceptOffer received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
}

BOOST_FIXTURE_TEST_CASE( netn2_ready_to_teceive_service, NetnV2Fixture )
{
    interactions::NetnReadyToReceiveService request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport

    ::hla::MockInteractionNotification< interactions::NetnReadyToReceiveService > notification;
    ::hla::Interaction< interactions::NetnReadyToReceiveService > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_ReadyToReceiveService", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnReadyToReceiveService received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
}

BOOST_FIXTURE_TEST_CASE( netn2_service_started, NetnV2Fixture )
{
    interactions::NetnServiceStarted request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport

    ::hla::MockInteractionNotification< interactions::NetnServiceStarted > notification;
    ::hla::Interaction< interactions::NetnServiceStarted > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_ServiceStarted", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnServiceStarted received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
}

BOOST_FIXTURE_TEST_CASE( netn2_service_complete, NetnV2Fixture )
{
    interactions::NetnServiceComplete request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport

    ::hla::MockInteractionNotification< interactions::NetnServiceComplete > notification;
    ::hla::Interaction< interactions::NetnServiceComplete > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.SCP_ServiceComplete", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnServiceComplete received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
}

BOOST_FIXTURE_TEST_CASE( netn2_trasport_embarkment_status, NetnV2Fixture )
{
    interactions::NetnConvoyEmbarkmentStatus request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport
    const unsigned char transportedUuid[] = { 0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70 };
    request.listOfObjectEmbarked.list = std::vector< NetnObjectDefinitionStruct >( 1, NetnObjectDefinitionStruct( "callsign", std::vector< char >( (char*)std::begin( transportedUuid ), (char*)std::end( transportedUuid ) ), NetnObjectFeatureStruct() ) );
    request.transportUnitIdentifier = UnicodeString("e41a7728-c203-4b8a-9c36-761eccf5c828");

    ::hla::MockInteractionNotification< interactions::NetnConvoyEmbarkmentStatus > notification;
    ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.TransportEmbarkmentStatus", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    CHECK_PARAMETER( "EmbarkedObjects", embarkedObjects, handler , { 0x00, 0x00, 0x00, 0x01,  0xA8, 0xE0, 0xE3, 0x82,  0x1B, 0x83, 0x47, 0xBA,  0x84, 0x6A, 0x84, 0x52,  0x54, 0x93, 0x2C, 0x70 } );
    CHECK_PARAMETER( "TransportUnitIdentifier", transportUnit, handler , { 0xE4, 0x1A, 0x77, 0x28,  0xC2, 0x03, 0x4B, 0x8A,  0x9C, 0x36, 0x76, 0x1E,  0xCC, 0xF5, 0xC8, 0x28 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnConvoyEmbarkmentStatus received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.listOfObjectEmbarked.list.size(), request.listOfObjectEmbarked.list.size() );
    BOOST_CHECK_EQUAL_COLLECTIONS( received.listOfObjectEmbarked.list[0].uniqueId.begin(), received.listOfObjectEmbarked.list[0].uniqueId.end(),
            request.listOfObjectEmbarked.list[0].uniqueId.begin(), request.listOfObjectEmbarked.list[0].uniqueId.end() );
    BOOST_CHECK_EQUAL( received.transportUnitIdentifier.str(), request.transportUnitIdentifier.str() );
}

BOOST_FIXTURE_TEST_CASE( netn2_trasport_disembarkment_status, NetnV2Fixture )
{
    interactions::NetnConvoyDisembarkmentStatus request;
    request.serviceId = NetnEventIdentifier( 80, "SWORD" );
    request.consumer = UnicodeString( "SWORD" );
    request.provider = UnicodeString( "ID_SIMU_001" );
    request.serviceType = 4; // Transport
    const unsigned char transportedUuid[] = { 0xA8, 0xE0, 0xE3, 0x82, 0x1B, 0x83, 0x47, 0xBA, 0x84, 0x6A, 0x84, 0x52, 0x54, 0x93, 0x2C, 0x70 };
    request.listOfObjectDisembarked.list = std::vector< NetnObjectDefinitionStruct >( 1, NetnObjectDefinitionStruct( "callsign", std::vector< char >( (char*)std::begin( transportedUuid ), (char*)std::end( transportedUuid ) ), NetnObjectFeatureStruct() ) );
    request.transportUnitIdentifier = UnicodeString("e41a7728-c203-4b8a-9c36-761eccf5c828");

    ::hla::MockInteractionNotification< interactions::NetnConvoyDisembarkmentStatus > notification;
    ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus > interaction( notification );

    MOCK_EXPECT( federate.RegisterInteraction ).once().with( "SCP_Service.TransportDisembarkmentStatus", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
    BOOST_CHECK( builder.Build( interaction ) );
    CHECK_PARAMETER( "ServiceID", serviceId, handler, { 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05, 0x00,0x53, 0x00, 0x57, 0x00, 0x4F, 0x00, 0x52, 0x00,0x44 });
    CHECK_PARAMETER( "Consumer", consumer, handler , { 0x00, 0x00, 0x00, 0x05, 0x00, 0x53, 0x00, 0x57, 0x00, 0x4F,0x00, 0x52, 0x00, 0x44 } );
    CHECK_PARAMETER( "Provider", provider, handler , { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x55, 0x00, 0x5F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x31 } );
    CHECK_PARAMETER( "ServiceType", svcType, handler , { 0x04 } );
    CHECK_PARAMETER( "DisembarkedObjects", disembarkedObjects, handler , { 0x00, 0x00, 0x00, 0x01,  0xA8, 0xE0, 0xE3, 0x82,  0x1B, 0x83, 0x47, 0xBA,  0x84, 0x6A, 0x84, 0x52,  0x54, 0x93, 0x2C, 0x70 } );
    CHECK_PARAMETER( "TransportUnitIdentifier", transportUnit, handler , { 0xE4, 0x1A, 0x77, 0x28,  0xC2, 0x03, 0x4B, 0x8A,  0x9C, 0x36, 0x76, 0x1E,  0xCC, 0xF5, 0xC8, 0x28 } );
    MOCK_EXPECT( handler->End ).once();

    interaction.Send( request );

    interaction.Create( serializedParameters );

    interactions::NetnConvoyDisembarkmentStatus received;
    MOCK_EXPECT( handler->Flush ).once();
    MOCK_EXPECT( notification.Receive ).once().with( mock::retrieve( boost::ref( received ) ) );
    interaction.Flush();

    BOOST_CHECK_EQUAL( received.serviceId.eventCount,request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( received.serviceId.issuingObjectIdentifier.str(),request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( received.consumer.str(), request.consumer.str() );
    BOOST_CHECK_EQUAL( received.provider.str(), request.provider.str() );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.serviceType, request.serviceType );
    BOOST_CHECK_EQUAL( received.listOfObjectDisembarked.list.size(), request.listOfObjectDisembarked.list.size() );
    BOOST_CHECK_EQUAL_COLLECTIONS( received.listOfObjectDisembarked.list[0].uniqueId.begin(), received.listOfObjectDisembarked.list[0].uniqueId.end(),
            request.listOfObjectDisembarked.list[0].uniqueId.begin(), request.listOfObjectDisembarked.list[0].uniqueId.end() );
    BOOST_CHECK_EQUAL( received.transportUnitIdentifier.str(), request.transportUnitIdentifier.str() );
}

