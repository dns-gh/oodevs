// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "MockEntityIdentifierResolver.h"
#include "MockObjectListener.h"
#include "SerializationFixture.h"
#include "hla_plugin_test_pch.h"
#include "hla_plugin/RawDataHazardContourGroup.h"
#include "hla_plugin/FOM_Serializer.h"
#include "hla_plugin/AttributesSerializer.h"
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

namespace
{
    struct Fixture : public SerializationFixture
    {
        Fixture()
            :   fomSerializer_( 2 )
        {
        }

        FOM_Serializer fomSerializer_;
        MockEntityIdentifierResolver entityIdResolver_;
    };
    struct RawDataHazardFixture : Fixture
    {
        RawDataHazardFixture()
            : rawData_( "rawData", entityIdResolver_, fomSerializer_ )
        {
            rawData_.Register( listener_ );
        }
        RawDataHazardContourGroup rawData_;
        MockObjectListener listener_;
    };
}

BOOST_FIXTURE_TEST_CASE( rawdata_receive_empty_contours_test, RawDataHazardFixture )
{
    std::vector< RawDataHazardContour > contours;

    PaddedVariableArray< RawDataHazardContour, 8  > contoursAttr;
    contoursAttr.list = contours;

    ::hla::Deserializer deser = Serialize( contoursAttr );
    rawData_.Deserialize( ::hla::AttributeIdentifier( "Contours" ), deser );
}

BOOST_FIXTURE_TEST_CASE( rawdata_receive_contours_test, RawDataHazardFixture )
{
    RawDataHazardContour contour0;
    contour0.exposureLevel = 42.f;
    contour0.locations.push_back( rpr::WorldLocation(42, 3, 0 ) );
    contour0.locations.push_back( rpr::WorldLocation(42, 4, 0 ) );
    contour0.locations.push_back( rpr::WorldLocation(43, 4, 0 ) );
    contour0.locations.push_back( rpr::WorldLocation(43, 3, 0 ) );
    std::vector< RawDataHazardContour > contours;
    contours.push_back( contour0 );

    struct Check
    {
        static void Verify( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
            int /*col*/, int /*lig*/, double xll, double yll, double, double )
        {
            BOOST_CHECK_EQUAL( rtiIdentifier, "rawData" );
            BOOST_CHECK_CLOSE( xll, 3., 0.001 );
            BOOST_CHECK_CLOSE( yll, 42., 0.001 );
        }
    };
    PaddedVariableArray< RawDataHazardContour, 8  > contoursAttr;
    contoursAttr.list = contours;
    ::hla::Deserializer deser = Serialize( contoursAttr );
    MOCK_EXPECT( listener_.PropagationChanged ).once().calls( &Check::Verify );
    rawData_.Deserialize( ::hla::AttributeIdentifier( "Contours" ), deser );
}



