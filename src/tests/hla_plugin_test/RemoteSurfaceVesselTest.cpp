// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SurfaceVessel.h"
#include "hla_plugin/Spatial.h"
#include "hla_plugin/AggregateMarking.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/SilentEntity.h"
#include "rpr/EntityType.h"
#include "MockUpdateFunctor.h"
#include "MockObjectListener.h"
#include "MockEntityIdentifierResolver.h"
#include "hla_plugin/FOM_Serializer.h"
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <vector>

using namespace plugins::hla;

namespace
{
    typedef std::vector< uint8_t > T_Buffer;
    class Fixture
    {
    public:
        Fixture()
            : fomSerialization( 1 )
            , vessel( "identifier", entityIdResolver, fomSerialization )
        {
            vessel.Register( listener );
        }
        ::hla::Deserializer Deserialize()
        {
            buffer.resize( serializer.GetSize() );
            serializer.CopyTo( &buffer[0] );
            return ::hla::Deserializer( &buffer[0], buffer.size() );
        }
        MockObjectListener listener;
        MockEntityIdentifierResolver entityIdResolver;
        FOM_Serializer fomSerialization;
        SurfaceVessel vessel;
        ::hla::Serializer serializer;
        T_Buffer buffer;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_vessel_can_be_serialized, Fixture )
{
    ::hla::MockUpdateFunctor functor;
    MOCK_EXPECT( functor.Visit );
    BOOST_CHECK_NO_THROW( vessel.Serialize( functor, true ) );
}

BOOST_FIXTURE_TEST_CASE( remote_vessel_deserializes_spatial_attribute_and_notifies_listener, Fixture )
{
    Spatial spatial( true, 1., 2., 3., 4., 5. );
    spatial.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
    MOCK_EXPECT( listener.Moved ).once().with( "identifier", mock::near( 1., 0.001 ), mock::near( 2., 0.001 ) );
    ::hla::Deserializer deserializer( Deserialize() );
    vessel.Deserialize( "Spatial", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_vessel_deserializes_force_identifier_attribute_and_notifies_listener, Fixture )
{
    serializer << static_cast< int8_t >( rpr::Friendly );
    MOCK_EXPECT( listener.SideChanged ).once().with( "identifier", rpr::Friendly );
    ::hla::Deserializer deserializer( Deserialize() );
    vessel.Deserialize( "ForceIdentifier", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_vessel_deserializes_vessel_marking_attribute_and_notifies_listener, Fixture )
{
    const Marking marking( "name", 42 );
    marking.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
    MOCK_EXPECT( listener.NameChanged ).once().with( "identifier", "name42" );
    ::hla::Deserializer deserializer( Deserialize() );
    vessel.Deserialize( "Marking", deserializer );
}

BOOST_FIXTURE_TEST_CASE( remote_vessel_deserializes_entity_type_attribute_and_notifies_listener, Fixture )
{
    const rpr::EntityType type( "1 2 3" );
    type.Serialize( serializer );
    MOCK_EXPECT( listener.TypeChanged ).once().with( "identifier", rpr::EntityType( "1 2 3 0 0 0 0" ) );
    MOCK_EXPECT( listener.EquipmentUpdated ).once().with( "identifier", rpr::EntityType( "1 2 3 0 0 0 0" ), 1u, 0u, 0u, 0u );
    ::hla::Deserializer deserializer( Deserialize() );
    vessel.Deserialize( "EntityType", deserializer );
}
