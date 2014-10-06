// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Minefield.h"
#include "hla_plugin/SerializationTools.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "MockTacticalObject.h"
#include "MockUpdateFunctor.h"
#include "SerializationFixture.h"
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
            : minefield( object, 42, "name", rpr::Friendly, rpr::EntityType(), entityId, "rtiId")
        {}
        Minefield minefield;
    };
}

BOOST_FIXTURE_TEST_CASE( minefield_can_deserialize, RegisteredFixture )
{
    hla::Deserializer deserializer( 0 );
    BOOST_CHECK_NO_THROW( minefield.Deserialize( "identifier", deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( minefield_serializes_all_its_attributes, RegisteredFixture )
{
    const std::vector< std::string > attributes = boost::assign::list_of( "MinefieldType" )
                                                                        ( "MinefieldIdentifier" )
                                                                        ( "ForceIdentifier" )
                                                                        ( "MinefieldLocation" )
                                                                        //( "MinefieldOrientation" )
                                                                        ( "MineTypes" )
                                                                        ( "PerimeterPointCoordinates" )
                                                                        ( "ProtocolMode" )
                                                                        ( "State" )
                                                                        ( "ActiveStatus" );
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        minefield.Serialize( functor, true );
    }
    {
        hla::MockUpdateFunctor functor;
        mock::sequence s;
        BOOST_FOREACH( const std::string& attribute, attributes )
            MOCK_EXPECT( functor.Visit ).once().in( s ).with( attribute, mock::any );
        minefield.Serialize( functor, true );
    }
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

BOOST_FIXTURE_TEST_CASE( minefield_has_entity_identifier, RegisteredFixture )
{
    const rpr::EntityIdentifier expectedId( 42, 43, 1);
    MOCK_EXPECT( functor.Visit ).once().with( "MinefieldIdentifier", boost::bind( &CheckSerialization< rpr::EntityIdentifier >, _1, expectedId ) );
    MOCK_EXPECT( functor.Visit );
    minefield.Serialize( functor, true );
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
}
BOOST_FIXTURE_TEST_CASE( minefield_serializes_perimeter, RegisteredFixture )
{
    const ObjectLocationEventListener_ABC::T_PositionVector perimeter = boost::assign::list_of( createCoord( 30, 28 ) )
                                                                                              ( createCoord( 30, 28.1 ) )
                                                                                              ( createCoord( 30.1, 28.1 ) )
                                                                                              ( createCoord( 30.1, 28 ) );

    BOOST_CHECK( listener != 0 );
    ObjectLocationEventListener_ABC* locationListener = dynamic_cast< ObjectLocationEventListener_ABC* >( listener );
    BOOST_CHECK( locationListener != 0 );

    rpr::WorldLocation center;
    locationListener->SpatialChanged( perimeter );
    MOCK_EXPECT( functor.Visit ).once().with( "MinefieldLocation", boost::bind( &GetValue< rpr::WorldLocation >, _1, boost::ref( center ) ) );
    MOCK_EXPECT( functor.Visit ).once().with( "PerimeterPointCoordinates", boost::bind( &CheckPerimeter, _1, boost::cref( perimeter ), boost::cref( center  ) ) );
    MOCK_EXPECT( functor.Visit );
    minefield.Serialize( functor, true );
}



