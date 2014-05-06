// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/BreachableLinearObject.h"
#include "hla_plugin/SerializationTools.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "MockTacticalObject.h"
#include "MockUpdateFunctor.h"
#include "MockObjectListener.h"
#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )
#include <geocoord/Geodetic.h>
#include <geocoord/PlanarCartesian.h>
#include <hla/Deserializer.h>
#include <hla/Serializer.h>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <numeric>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : listener( 0 )
            , entityId( 42, 43, 1 )
        {
            MOCK_EXPECT( object.Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( object.Unregister ).once();
        }
        MockTacticalObject object;
        TacticalObjectEventListener_ABC* listener;
        rpr::EntityType entityType;
        rpr::EntityIdentifier entityId;
        hla::MockUpdateFunctor functor;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : linearObject( object, 42, "name", rpr::Friendly, rpr::EntityType(), entityId, "rtiId")
        {}
        BreachableLinearObject linearObject;
    };
}

BOOST_FIXTURE_TEST_CASE( linearobject_can_deserialize, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_NO_THROW( linearObject.Deserialize( "identifier", deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( linearobject_serializes_all_its_attributes, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "ObjectType" )
                                                                        ( "ObjectIdentifier" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "SegmentRecords" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        linearObject.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        linearObject.Serialize( functor, true );
    }
}

namespace rpr
{
    template <typename Archive>
    void operator >> (  Archive& archive, EntityIdentifier& id )
    {
        id.Deserialize( archive );
    }
    std::ostream& operator<< ( std::ostream& os, const EntityIdentifier& id );
    std::ostream& operator<< ( std::ostream& os, const WorldLocation& loc );
    bool operator== (const WorldLocation& lhs, const WorldLocation& rhs );
    bool operator== (const Orientation& lhs, const Orientation& rhs );
}

namespace
{
    template< typename T >
    bool CheckSerialization( ::hla::T_SerializerPtr serializer, const T& expected )
    {
        std::vector< uint8_t > buffer( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        T actual;
        deserializer >> actual;
        BOOST_CHECK_EQUAL( expected, actual );
        BOOST_CHECK_THROW( deserializer >> actual, std::exception );
        return true;
    }
    bool CheckSize( ::hla::T_SerializerPtr serializer, unsigned int size )
    {
        BOOST_CHECK_EQUAL( size, serializer->GetSize() );
        return true;
    }
    const unsigned int SILENT_ENTITY_SIZE = 2 * sizeof( int16_t ) + 6 * sizeof( int8_t ) + 1 * sizeof( int16_t ) + sizeof( int32_t );
}

BOOST_FIXTURE_TEST_CASE( linearobject_has_entity_identifier, RegisteredFixture )
{
    const rpr::EntityIdentifier expectedId( 42, 43, 1);
    MOCK_EXPECT( functor.Visit ).once().with( "ObjectIdentifier", boost::bind( &CheckSerialization< rpr::EntityIdentifier >, _1, expectedId ) );
    MOCK_EXPECT( functor.Visit );
    linearObject.Serialize( functor, true );
}

namespace
{
    sword::CoordLatLong createCoord( double lat, double lon )
    {
        sword::CoordLatLong retval;
        retval.set_latitude( lat );
        retval.set_longitude( lon );
        return retval;
    }
    bool CheckPerimeter( ::hla::T_SerializerPtr serializer, const ObjectLocationEventListener_ABC::T_PositionVector& p,
        const rpr::WorldLocation& center )
    {
        static const double rPiOver180 = std::acos( -1. ) / 180.;
        std::vector< uint8_t > buffer( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        std::vector< rpr::PerimeterPoint > locs;
        deserializer >> locs;
        BOOST_CHECK_EQUAL( locs.size(), p.size() );
        geocoord::PlanarCartesian::Parameters params; params.SetOrigin( center.Latitude() * rPiOver180, center.Longitude() * rPiOver180);
        for( std::size_t i = 0; i< locs.size(); ++i )
        {
            ObjectLocationEventListener_ABC::T_PositionVector::const_reference pRef = p[i];
            std::vector< rpr::PerimeterPoint >::const_reference pExt = locs[i];
            geocoord::PlanarCartesian loc( pExt.X(), pExt.Y(), 0, params );
            geocoord::Geodetic wl( loc );
            BOOST_CHECK_CLOSE( pRef.latitude(), wl.GetLatitude() / rPiOver180, 1e-6 );
            BOOST_CHECK_CLOSE( pRef.longitude(), wl.GetLongitude() / rPiOver180, 1e-6 );
        }
        return true;
    }
    template< typename T >
    bool GetValue( ::hla::T_SerializerPtr serializer, T& v )
    {
        std::vector< uint8_t > buffer( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        v.Deserialize( deserializer );
        return true;
    }
    bool GetBuffer( ::hla::T_SerializerPtr serializer, std::vector< uint8_t >& buffer )
    {
        buffer.resize( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        return true;
    }
}
BOOST_FIXTURE_TEST_CASE( linearobject_serializes_location, RegisteredFixture )
{
    const ObjectLocationEventListener_ABC::T_PositionVector perimeter = boost::assign::list_of( createCoord( 30, 28 ) )
                                                                                              ( createCoord( 30, 28.1 ) )
                                                                                              ( createCoord( 30.1, 28.1 ) )
                                                                                              ( createCoord( 30.1, 28 ) );
    BOOST_CHECK( listener != 0 );
    ObjectLocationEventListener_ABC* locationListener = dynamic_cast< ObjectLocationEventListener_ABC* >( listener );
    BOOST_CHECK( locationListener != 0 );

    std::vector< uint8_t > buffer;
    locationListener->SpatialChanged( perimeter );
    MOCK_EXPECT( functor.Visit ).once().with( "SegmentRecords", boost::bind( &GetBuffer, _1, boost::ref( buffer ) ) );
    MOCK_EXPECT( functor.Visit );
    linearObject.Serialize( functor, true );

    ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    std::vector< rpr::WorldLocation > points;
    MockObjectListener objectListener;
    linearObject.Register( objectListener );
    MOCK_EXPECT( objectListener.GeometryChanged ).once().with( "rtiId", mock::retrieve( points ), ObjectListener_ABC::eGeometryType_Line );
    linearObject.Deserialize( "SegmentRecords", deserializer );
    BOOST_CHECK_EQUAL( points.size(), perimeter.size() );
    for( std::size_t i=0; i < perimeter.size(); ++i )
    {
        BOOST_CHECK_CLOSE( points[i].Latitude(), perimeter[i].latitude(), 1e-4 );
        BOOST_CHECK_CLOSE( points[i].Longitude(), perimeter[i].longitude(), 1e-4 );
    }

    // cleanup
    linearObject.Unregister( objectListener );
}



