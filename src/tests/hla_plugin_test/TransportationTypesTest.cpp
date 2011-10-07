// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Transportation.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( netn_object_feature_struct_deserialization, SerializationFixture )
{
    NetnObjectFeatureStruct serializedFeature;
    const unsigned int discriminantTypeSize = sizeof( int32 );
    ::hla::Deserializer deserializer = Serialize( serializedFeature, discriminantTypeSize );
    NetnObjectFeatureStruct deserializedFeature;
    deserializedFeature.Deserialize( deserializer );
    const int32 noDetail = 0;
    BOOST_CHECK_EQUAL( noDetail, deserializedFeature.featureLevel );
}

BOOST_FIXTURE_TEST_CASE( netn_object_definition_struct_deserialization, SerializationFixture )
{
    const std::string callsign = "callsign";
    const std::string uniqueId = "uniqueid";
    const NetnObjectDefinitionStruct serializedObjectDefinition( callsign, uniqueId );
    const unsigned int callsignSize = sizeof( uint32 ) + callsign.size() * sizeof( int16 );
    const unsigned int uniqueIdSize = 11 * sizeof( int8 );
    const unsigned int objectFeatureSize = sizeof( int32 );
    ::hla::Deserializer deserializer = Serialize( serializedObjectDefinition, callsignSize + uniqueIdSize + objectFeatureSize );
    NetnObjectDefinitionStruct deserializedObjectDefinition;
    deserializedObjectDefinition.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedObjectDefinition.callsign.str(), callsign );
    BOOST_CHECK_EQUAL( deserializedObjectDefinition.uniqueId.str(), uniqueId );
}

BOOST_FIXTURE_TEST_CASE( netn_appointment_struct_deserialization, SerializationFixture )
{
    const int64 dateTime = 10021123;
    const rpr::WorldLocation location( 1., 2., 3. );
    const NetnAppointmentStruct serializedAppointmentStruct( dateTime, location );
    const unsigned int dateTimeSize = sizeof( int64 );
    const unsigned int worldLocationSize = 3 * sizeof( real64 );
    ::hla::Deserializer deserializer = Serialize( serializedAppointmentStruct, dateTimeSize + worldLocationSize );
    NetnAppointmentStruct deserializedAppointmentStruct;
    deserializedAppointmentStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedAppointmentStruct.dateTime, 10021123 );
    BOOST_CHECK_CLOSE( deserializedAppointmentStruct.location.Latitude() , 1., 0.00001 );
    BOOST_CHECK_CLOSE( deserializedAppointmentStruct.location.Longitude(), 2., 0.00001 );
    BOOST_CHECK_CLOSE( deserializedAppointmentStruct.location.Altitude() , 3., 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( netn_datat_struct_deserialization, SerializationFixture )
{
    std::vector< NetnObjectDefinitionStruct > objectToManage;
    objectToManage.push_back( NetnObjectDefinitionStruct() );
    const NetnAppointmentStruct appointment;
    const NetnAppointmentStruct finalAppointment;
    const NetnDataTStruct serializedNetnDataTStruct( objectToManage, appointment, finalAppointment );
    const unsigned int objectToManageVectorSize = sizeof( int32 );
    const unsigned int objectDefinitionStructSize = sizeof( int32 ) + 11 * sizeof( int8 ) + sizeof( int32 );
    const unsigned int appointmentStructSize = sizeof( int64 ) + 3 * sizeof( real64 );
    ::hla::Deserializer deserializer = Serialize( serializedNetnDataTStruct, objectToManageVectorSize + 1 * objectDefinitionStructSize + 2 * appointmentStructSize );
    NetnDataTStruct deserializedNetnDataTStruct;
    deserializedNetnDataTStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedNetnDataTStruct.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( deserializedNetnDataTStruct.finalAppointment.dateTime, 0u );
}

BOOST_FIXTURE_TEST_CASE( netn_event_identifier_deserialization, SerializationFixture )
{
    const int32 eventCount = 42;
    const std::string callsign( "callsign" );
    const unsigned int eventCountSize = sizeof( int32 );
    const unsigned int callsignSize = sizeof( int32 ) + sizeof( int16 ) * callsign.size();
    const NetnEventIdentifier serializedEventIdentifier( eventCount, callsign );
    ::hla::Deserializer deserializer = Serialize( serializedEventIdentifier, eventCountSize + callsignSize );
    NetnEventIdentifier deserializedEventIdentifier;
    deserializedEventIdentifier.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedEventIdentifier.eventCount, eventCount );
    BOOST_CHECK_EQUAL( deserializedEventIdentifier.issuingObjectIdentifier.str(), callsign );
}

BOOST_FIXTURE_TEST_CASE( netn_transport_struct_deserialization, SerializationFixture )
{
    const NetnTransportStruct serializedTransport;
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, serializedTransport.convoyType );
}
