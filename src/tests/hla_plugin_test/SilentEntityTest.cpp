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
    const SilentEntity entity( type, numberOfEntitiesOfThisType );
    const unsigned int entityTypeSize = 6 * sizeof( int8_t ) + 1 * sizeof( int16_t );
    ::hla::Deserializer deserializer = Serialize( entity, 2 * sizeof( int16_t ) + entityTypeSize );
    BOOST_CHECK_EQUAL( numberOfEntitiesOfThisType, Read< uint16_t >( deserializer ) );
    BOOST_CHECK_EQUAL( numberOfAppearanceRecords , Read< uint16_t >( deserializer ) );
    Read< entityTypeSize >( deserializer );
}

BOOST_FIXTURE_TEST_CASE( silent_entity_deserialization, SerializationFixture )
{
    const unsigned int numberOfEntitiesOfThisType = 3u;
    const rpr::EntityType type;
    const SilentEntity serializedEntity( type, numberOfEntitiesOfThisType );
    const unsigned int entityTypeSize = 6 * sizeof( int8_t ) + 1 * sizeof( int16_t );
    ::hla::Deserializer deserializer = Serialize( serializedEntity, 2 * sizeof( int16_t ) + entityTypeSize );
    SilentEntity deserializedEntity;
    deserializedEntity.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedEntity.numberOfEntitiesOfThisType_, deserializedEntity.numberOfEntitiesOfThisType_ );
    BOOST_CHECK_EQUAL( serializedEntity.entityType_.str()          , deserializedEntity.entityType_.str() );
}

BOOST_FIXTURE_TEST_CASE( silent_entity_serialization_with_appearance, SerializationFixture )
{
    const unsigned int numberOfEntitiesOfThisType = 3u;
    const unsigned int numberOfAppearanceRecords = 1u;
    const rpr::EntityType type;
    const uint32_t entityAppearance = 42u;
    SilentEntity entity( type, numberOfEntitiesOfThisType );
    entity.entityAppearance_.push_back( entityAppearance );
    const unsigned int entityTypeSize = 6 * sizeof( int8_t ) + 1 * sizeof( int16_t );
    ::hla::Deserializer deserializer = Serialize( entity, 2 * sizeof( int16_t ) + entityTypeSize + sizeof( uint32_t ) );
    BOOST_CHECK_EQUAL( numberOfEntitiesOfThisType, Read< uint16_t >( deserializer ) );
    BOOST_CHECK_EQUAL( numberOfAppearanceRecords , Read< uint16_t >( deserializer ) );
    Read< entityTypeSize >( deserializer );
    BOOST_CHECK_EQUAL( entityAppearance          , Read< uint32_t >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( silent_entity_deserialization_with_appearance, SerializationFixture )
{
    const unsigned int numberOfEntitiesOfThisType = 3u;
    const rpr::EntityType type;
    const uint32_t entityAppearance = 42u;
    SilentEntity serializedEntity( type, numberOfEntitiesOfThisType );
    serializedEntity.entityAppearance_.push_back( entityAppearance );
    const unsigned int entityTypeSize = 6 * sizeof( int8_t ) + 1 * sizeof( int16_t );
    ::hla::Deserializer deserializer = Serialize( serializedEntity, 2 * sizeof( int16_t ) + entityTypeSize + sizeof( uint32_t ) );
    SilentEntity deserializedEntity;
    deserializedEntity.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedEntity.numberOfEntitiesOfThisType_, deserializedEntity.numberOfEntitiesOfThisType_ );
    BOOST_CHECK_EQUAL( serializedEntity.entityType_.str()          , deserializedEntity.entityType_.str() );
    BOOST_CHECK_EQUAL( serializedEntity.entityAppearance_.size()   , deserializedEntity.entityAppearance_.size() );
    BOOST_CHECK_EQUAL( serializedEntity.entityAppearance_[0]       , deserializedEntity.entityAppearance_[0] );
}
