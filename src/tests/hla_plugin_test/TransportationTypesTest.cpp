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

BOOST_FIXTURE_TEST_CASE( netn_object_description_deserialization, SerializationFixture )
{
    const real32 weight = 1.;
    const real32 volume = 2.;
    const std::string type = "type";
    NetnObjectDescription serializedDescription( weight, volume, type );
    const unsigned int descriptionSize = sizeof( real32 ) + sizeof( real32 ) + sizeof( int32 ) + sizeof( int8 ) * type.size();
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnObjectDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.volume, volume );
    BOOST_CHECK_EQUAL( deserializedDescription.weight, weight );
    BOOST_CHECK_EQUAL( deserializedDescription.type, type );
}

BOOST_FIXTURE_TEST_CASE( netn_human_description_deserialization, SerializationFixture )
{
    const rpr::EntityType humanType( "1 2 3 0 0" );
    const int16 quantity = 1;
    const int16 injury = 3;
    NetnHumanDescription serializedDescription( humanType, quantity, injury );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int16 ) + sizeof( int16 );
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnHumanDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.humanType, humanType );
    BOOST_CHECK_EQUAL( deserializedDescription.quantity, quantity );
    BOOST_CHECK_EQUAL( deserializedDescription.injury, injury );
}

BOOST_FIXTURE_TEST_CASE( netn_equip_description_deserialization, SerializationFixture )
{
    const rpr::EntityType equipType( "1 2 3 0 0" );
    const int32 quantity = 1;
    const int32 damageState = 3;
    NetnEquipDescription serializedDescription( equipType, quantity, damageState );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int32 ) + sizeof( int32 );
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnEquipDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.equipType, equipType );
    BOOST_CHECK_EQUAL( deserializedDescription.damageState, damageState );
}

BOOST_FIXTURE_TEST_CASE( netn_platform_description_deserialization, SerializationFixture )
{
    const rpr::EntityType plateformType( "1 2 3 0 0" );
    const int32 damageState = 3;
    NetnPlateformDescription serializedDescription( plateformType, damageState );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int32 );
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnPlateformDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.plateformType, plateformType );
    BOOST_CHECK_EQUAL( deserializedDescription.damageState, damageState );
}

BOOST_FIXTURE_TEST_CASE( netn_object_feature_struct_no_detail_deserialization, SerializationFixture )
{
    NetnObjectFeatureStruct serializedFeature;
    const unsigned int discriminantTypeSize = sizeof( int32 );
    ::hla::Deserializer deserializer = Serialize( serializedFeature, discriminantTypeSize );
    NetnObjectFeatureStruct deserializedFeature;
    deserializedFeature.Deserialize( deserializer );
    const int32 noDetail = 0;
    BOOST_CHECK_EQUAL( noDetail, deserializedFeature.featureLevel );
}

BOOST_FIXTURE_TEST_CASE( netn_object_feature_struct_with_aggregate_deserialization, SerializationFixture )
{
    std::vector< NetnObjectDefinitionStruct > subObjectList;
    subObjectList.push_back( NetnObjectDefinitionStruct( "callsign", "uniqueid", NetnObjectFeatureStruct() ) );
    NetnObjectFeatureStruct serializedFeature( subObjectList );
    ::hla::Deserializer deserializer = Serialize( serializedFeature );
    NetnObjectFeatureStruct deserializedFeature;
    deserializedFeature.Deserialize( deserializer );
    const int32 aggregateDetail = 5;
    BOOST_CHECK_EQUAL( aggregateDetail, deserializedFeature.featureLevel );
    BOOST_CHECK_EQUAL( 1u, deserializedFeature.subObjectList.size() );
    BOOST_CHECK_EQUAL( "callsign", deserializedFeature.subObjectList.at( 0 ).callsign.str() );
}

BOOST_FIXTURE_TEST_CASE( netn_object_feature_constructors, SerializationFixture )
{
    const NetnObjectDescription objectDescription;
    const NetnHumanDescription humanDescription;
    const NetnEquipDescription equipDescription;
    const NetnPlateformDescription plateformDescription;
    NetnObjectFeatureStruct object( objectDescription );
    NetnObjectFeatureStruct human( humanDescription );
    NetnObjectFeatureStruct equipment( equipDescription );
    NetnObjectFeatureStruct plateform( plateformDescription );
    BOOST_CHECK_EQUAL( object.featureLevel, 1 );
    BOOST_CHECK_EQUAL( human.featureLevel, 2 );
    BOOST_CHECK_EQUAL( equipment.featureLevel, 3 );
    BOOST_CHECK_EQUAL( plateform.featureLevel, 4 );
}

BOOST_FIXTURE_TEST_CASE( netn_object_definition_struct_deserialization, SerializationFixture )
{
    const std::string callsign = "callsign";
    const std::string uniqueId = "uniqueid";
    const NetnObjectDefinitionStruct serializedObjectDefinition( callsign, uniqueId, NetnObjectFeatureStruct() );
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

BOOST_FIXTURE_TEST_CASE( netn_dataed_struct_deserialization, SerializationFixture )
{
    std::vector< NetnObjectDefinitionStruct > objectToManage;
    objectToManage.push_back( NetnObjectDefinitionStruct() );
    const NetnAppointmentStruct appointment;
    const NetnDataEDStruct serializedNetnDataEDStruct( objectToManage, appointment );
    const unsigned int objectToManageVectorSize = sizeof( int32 );
    const unsigned int objectDefinitionStructSize = sizeof( int32 ) + 11 * sizeof( int8 ) + sizeof( int32 );
    const unsigned int appointmentStructSize = sizeof( int64 ) + 3 * sizeof( real64 );
    ::hla::Deserializer deserializer = Serialize( serializedNetnDataEDStruct, objectToManageVectorSize + 1 * objectDefinitionStructSize + appointmentStructSize );
    NetnDataEDStruct deserializedNetnDataEDStruct;
    deserializedNetnDataEDStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedNetnDataEDStruct.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( deserializedNetnDataEDStruct.appointment.dateTime, 0u );
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

BOOST_FIXTURE_TEST_CASE( netn_transport_struct_transport_deserialization, SerializationFixture )
{
    const NetnDataTStruct dataTransport;
    const NetnTransportStruct serializedTransport( dataTransport );
    BOOST_CHECK_EQUAL( serializedTransport.convoyType, 0 );
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, 0 );
}

BOOST_FIXTURE_TEST_CASE( netn_transport_struct_embarkment_deserialization, SerializationFixture )
{
    const NetnDataEDStruct dataEmbarkment;
    const NetnTransportStruct serializedTransport( dataEmbarkment, 1 );
    BOOST_CHECK_EQUAL( serializedTransport.convoyType, 1 );
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, 1 );
}

BOOST_FIXTURE_TEST_CASE( netn_transport_struct_disembarkment_deserialization, SerializationFixture )
{
    const NetnDataEDStruct dataDisembarkment;
    const NetnTransportStruct serializedTransport( dataDisembarkment, 2 );
    BOOST_CHECK_EQUAL( serializedTransport.convoyType, 2 );
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, 2 );
}
