// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SilentEntity.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( silent_entity_serialization, SerializationFixture )
{
    const unsigned int numberOfEntitiesOfThisType = 3u;
    const unsigned int numberOfAppearanceRecords = 0u;
    const rpr::EntityType type;
    const unsigned int entityAppearance = 0u;
    const SilentEntity entity( type, numberOfEntitiesOfThisType );
    const unsigned int entityTypeSize = 6 * sizeof( int8 ) + 1 * sizeof( int16 );
    ::hla::Deserializer deserializer = Serialize( entity, 2 * sizeof( int16 ) + entityTypeSize + sizeof( int32 ) );
    BOOST_CHECK_EQUAL( numberOfEntitiesOfThisType, Read< uint16 >( deserializer ) );
    BOOST_CHECK_EQUAL( numberOfAppearanceRecords , Read< uint16 >( deserializer ) );
    Read< entityTypeSize >( deserializer );
    BOOST_CHECK_EQUAL( entityAppearance          , Read< uint32 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( silent_entity_deserialization, SerializationFixture )
{
    const unsigned int numberOfEntitiesOfThisType = 3u;
    const rpr::EntityType type;
    const SilentEntity serializedEntity( type, numberOfEntitiesOfThisType );
    const unsigned int entityTypeSize = 6 * sizeof( int8 ) + 1 * sizeof( int16 );
    ::hla::Deserializer deserializer = Serialize( serializedEntity, 2 * sizeof( int16 ) + entityTypeSize + sizeof( int32 ) );
    SilentEntity deserializedEntity;
    deserializedEntity.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedEntity.numberOfEntitiesOfThisType_, deserializedEntity.numberOfEntitiesOfThisType_ );
    BOOST_CHECK_EQUAL( serializedEntity.numberOfAppearanceRecords_ , deserializedEntity.numberOfAppearanceRecords_ );
    BOOST_CHECK_EQUAL( serializedEntity.entityType_.str()          , deserializedEntity.entityType_.str() );
    BOOST_CHECK_EQUAL( serializedEntity.entityAppearance_          , deserializedEntity.entityAppearance_ );
}
