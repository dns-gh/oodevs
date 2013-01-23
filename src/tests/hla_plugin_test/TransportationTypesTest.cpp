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
    const unsigned int descriptionSize = 
        static_cast< unsigned int >( sizeof( real32 ) + sizeof( real32 ) + sizeof( int32_t ) + sizeof( int8_t ) * type.size() );
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
    const int16_t quantity = 1;
    const int16_t injury = 3;
    NetnHumanDescription serializedDescription( humanType, quantity, injury );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int16_t ) + sizeof( int16_t );
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
    const int32_t quantity = 1;
    const int32_t damageState = 3;
    NetnEquipDescription serializedDescription( equipType, quantity, damageState );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int32_t ) + sizeof( int32_t );
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnEquipDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.equipType, equipType );
    BOOST_CHECK_EQUAL( deserializedDescription.damageState, damageState );
}

BOOST_FIXTURE_TEST_CASE( netn_platform_description_deserialization, SerializationFixture )
{
    const rpr::EntityType plateformType( "1 2 3 0 0" );
    const int32_t damageState = 3;
    NetnPlateformDescription serializedDescription( plateformType, damageState );
    const unsigned int descriptionSize = 6 * sizeof( char ) + sizeof( short ) + sizeof( int32_t );
    ::hla::Deserializer deserializer = Serialize( serializedDescription, descriptionSize );
    NetnPlateformDescription deserializedDescription;
    deserializedDescription.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedDescription.plateformType, plateformType );
    BOOST_CHECK_EQUAL( deserializedDescription.damageState, damageState );
}

BOOST_FIXTURE_TEST_CASE( netn_object_feature_struct_no_detail_deserialization, SerializationFixture )
{
    NetnObjectFeatureStruct serializedFeature;
    const unsigned int discriminantTypeSize = sizeof( int32_t );
    ::hla::Deserializer deserializer = Serialize( serializedFeature, discriminantTypeSize );
    NetnObjectFeatureStruct deserializedFeature;
    deserializedFeature.Deserialize( deserializer );
    const int32_t noDetail = 0;
    BOOST_CHECK_EQUAL( noDetail, deserializedFeature.featureLevel );
}

BOOST_FIXTURE_TEST_CASE( netn_object_feature_struct_with_aggregate_deserialization, SerializationFixture )
{
    std::vector< NetnObjectDefinitionStruct > subObjectList;
    subObjectList.push_back( NetnObjectDefinitionStruct( "callsign", MakeUniqueId( "uniqueId" ), NetnObjectFeatureStruct() ) );
    NetnObjectFeatureStruct serializedFeature( subObjectList );
    ::hla::Deserializer deserializer = Serialize( serializedFeature );
    NetnObjectFeatureStruct deserializedFeature;
    deserializedFeature.Deserialize( deserializer );
    const int32_t aggregateDetail = 5;
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
    const std::vector< char > uniqueId ( MakeUniqueId( "uniqueId" ) );
    const unsigned int padding = sizeof( int8_t );
    const NetnObjectDefinitionStruct serializedObjectDefinition( callsign, uniqueId, NetnObjectFeatureStruct() );
    const unsigned int callsignSize = static_cast< unsigned int >( sizeof( uint32_t ) + callsign.size() * sizeof( int16_t ) );
    const unsigned int uniqueIdSize = 11 * sizeof( int8_t ) + padding;
    const unsigned int objectFeatureSize = sizeof( int32_t );
    ::hla::Deserializer deserializer = Serialize( serializedObjectDefinition, callsignSize + uniqueIdSize + objectFeatureSize );
    NetnObjectDefinitionStruct deserializedObjectDefinition;
    deserializedObjectDefinition.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedObjectDefinition.callsign.str(), callsign );
    BOOST_CHECK( deserializedObjectDefinition.uniqueId == uniqueId );
}

BOOST_FIXTURE_TEST_CASE( netn_appointment_struct_deserialization, SerializationFixture )
{
    const int64_t dateTime = 10021123;
    const rpr::WorldLocation location( 1., 2., 3. );
    const NetnAppointmentStruct serializedAppointmentStruct( dateTime, location );
    const unsigned int dateTimeSize = sizeof( int64_t );
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
    std::string callsign("callsign3");
    objectToManage.push_back( NetnObjectDefinitionStruct( callsign, MakeUniqueId( "42" ), NetnObjectFeatureStruct() ) );
    const NetnAppointmentStruct appointment;
    const NetnAppointmentStruct finalAppointment;
    const NetnDataTStruct serializedNetnDataTStruct( objectToManage, appointment, finalAppointment );
    const unsigned int padding = sizeof( int8_t );
    const unsigned int objectToManageVectorSize = sizeof( int32_t );
    const unsigned int objectDefinitionStructSize = static_cast< unsigned int >( 
        sizeof( int32_t ) + callsign.size() * sizeof( int16_t ) + 11 * sizeof( int8_t ) + padding + 2 * padding + sizeof( int32_t ) );
    const unsigned int appointmentStructSize = sizeof( int64_t ) + 3 * sizeof( real64 );
    const unsigned int alignment  = ( ( objectToManageVectorSize + 1 * objectDefinitionStructSize) % sizeof( int64_t ) ) * padding;
    ::hla::Deserializer deserializer = Serialize( serializedNetnDataTStruct, objectToManageVectorSize + 1 * objectDefinitionStructSize + alignment + 2 * appointmentStructSize );
    NetnDataTStruct deserializedNetnDataTStruct;
    deserializedNetnDataTStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedNetnDataTStruct.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( deserializedNetnDataTStruct.finalAppointment.dateTime, 0u );
    BOOST_CHECK_EQUAL( deserializedNetnDataTStruct.objectToManage[0].callsign.str(), callsign );
}

BOOST_FIXTURE_TEST_CASE( netn_dataed_struct_deserialization, SerializationFixture )
{
    std::vector< NetnObjectDefinitionStruct > objectToManage;
    std::string callsign("callsign3");
    objectToManage.push_back( NetnObjectDefinitionStruct( callsign, MakeUniqueId( "42" ), NetnObjectFeatureStruct() ) );
    const NetnAppointmentStruct appointment;
    const NetnDataEDStruct serializedNetnDataEDStruct( objectToManage, appointment );
    const unsigned int padding = sizeof( int8_t );
    const unsigned int objectToManageVectorSize = sizeof( int32_t );
    const unsigned int objectDefinitionStructSize = static_cast< unsigned int >(
        sizeof( int32_t ) + callsign.size() * sizeof( int16_t ) + 11 * sizeof( int8_t ) + padding + 2 * padding + sizeof( int32_t ) );
    const unsigned int appointmentStructSize = sizeof( int64_t ) + 3 * sizeof( real64 );
    const unsigned int alignment  = ( ( objectToManageVectorSize + 1 * objectDefinitionStructSize) % sizeof( int64_t ) ) * padding;
    ::hla::Deserializer deserializer = Serialize( serializedNetnDataEDStruct, objectToManageVectorSize + 1 * objectDefinitionStructSize + alignment + appointmentStructSize );
    NetnDataEDStruct deserializedNetnDataEDStruct;
    deserializedNetnDataEDStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedNetnDataEDStruct.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( deserializedNetnDataEDStruct.appointment.dateTime, 0u );
    BOOST_CHECK_EQUAL( deserializedNetnDataEDStruct.objectToManage[0].callsign.str(), callsign );
}

BOOST_FIXTURE_TEST_CASE( netn_event_identifier_deserialization, SerializationFixture )
{
    const int32_t eventCount = 42;
    const std::string callsign( "callsign" );
    const unsigned int eventCountSize = sizeof( int32_t );
    const unsigned int callsignSize = static_cast< unsigned int >( sizeof( int32_t ) + sizeof( int16_t ) * callsign.size() );
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
    const NetnTransportStruct serializedTransport( dataEmbarkment, NetnTransportStruct::E_Embarkment );
    BOOST_CHECK_EQUAL( serializedTransport.convoyType, 1 );
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, NetnTransportStruct::E_Embarkment );
}

BOOST_FIXTURE_TEST_CASE( netn_transport_struct_disembarkment_deserialization, SerializationFixture )
{
    const NetnDataEDStruct dataDisembarkment;
    const NetnTransportStruct serializedTransport( dataDisembarkment, NetnTransportStruct::E_Disembarkment );
    BOOST_CHECK_EQUAL( serializedTransport.convoyType, 2 );
    ::hla::Deserializer deserializer = Serialize( serializedTransport );
    NetnTransportStruct deserializedTransport;
    deserializedTransport.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedTransport.convoyType, NetnTransportStruct::E_Disembarkment );
}
