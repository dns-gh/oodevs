// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Meteo/RawVisionData/ElevationGrid.h"
#include "simulation_kernel/Meteo/RawVisionData/PHY_RawVisionData.h"
#include "simulation_terrain/TER_Localisation.h"
#include "meteo/Meteo.h"
#include "StubTER_World.h"

BOOST_AUTO_TEST_CASE( phy_rawvisiondata_cell_non_regression )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    weather::Meteo meteo;
    tools::Path detectionFile = testOptions.GetDataPath(
            "../../data/terrains/Paris_Est/Detection/detection.dat" );
    PHY_RawVisionData vision( meteo, detectionFile );

    const MT_Vector2D p1( 34020.52, 70833.78 );
    const auto& c1 = vision( p1 );
    BOOST_CHECK_EQUAL( c1.GetAltitude(), 109 );
    BOOST_CHECK_EQUAL( c1.GetEnv(), 1 );

    const MT_Vector2D p2( 91211.93, 46216.39 );
    const auto& c2 = vision( p2 );
    BOOST_CHECK_EQUAL( c2.GetAltitude(), 213 );
    BOOST_CHECK_EQUAL( c2.GetEnv(), 0 );
}

BOOST_AUTO_TEST_CASE( phy_rawvisiondata_getaltitude_offsets )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    weather::Meteo meteo;
    tools::Path detectionFile = testOptions.GetDataPath( "../../data/terrains/Paris_Est/Detection/detection.dat" );
    PHY_RawVisionData vision( meteo, detectionFile );

    T_PointVector points( 4 );
    points[ 0 ] = MT_Vector2D( 0.5, 0.5 );
    points[ 1 ] = MT_Vector2D( 0.5, 1.5 );
    points[ 2 ] = MT_Vector2D( 1.5, 1.5 );
    points[ 3 ] = MT_Vector2D( 1.5, 0.5 );

    // Required so (1, 2) is outside our polygon but inside the cell
    // neighboorhood.
    BOOST_REQUIRE_LT( 1, 0.5*vision.GetCellSize() );

    // Test added polygon altitude, exact collision
    const double refalt = vision.GetAltitude( 1, 1 );
    const double refalt2 = vision.GetAltitude( 1000, 1 );
    const double refalt3 = vision.GetAltitude( 1, 2 );
    const TER_Localisation rect( TER_Localisation::ePolygon, points );
    const short offset = 100;
    vision.ModifyAltitude( rect, offset, 1 );
    double alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( offset, alt - refalt, 0.1 );

    // Test polygon altitude, cell level collision
    alt = vision.GetAltitude( 1, 2, true );
    BOOST_CHECK_CLOSE( offset, alt - refalt3, 0.1 );

    // Test altitudes are not additive
    const short offset2 = 150;
    vision.ModifyAltitude( rect, offset2, 2 );
    alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( offset2, alt - refalt, 1.1 );

    // Test polygon with applyOnCell == false
    alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( offset2, alt - refalt, 1.1 );

    // Test polygon without collision
    alt = vision.GetAltitude( 1000, 1, true );
    BOOST_CHECK_CLOSE( 0, alt - refalt2, 1.1 );

    // Test polygon unregistration
    vision.ModifyAltitude( rect, 0, 2 );
    alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( offset, alt - refalt, 0.1 );
    vision.ModifyAltitude( rect, 0, 1 );
    alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( 0, alt - refalt, 0.1 );

    // Test added line altitude
    const TER_Localisation line( TER_Localisation::eLine, points );
    vision.ModifyAltitude( line, offset, 1 );
    alt = vision.GetAltitude( 1, 1, true );
    BOOST_CHECK_CLOSE( offset, alt - refalt, 0.1 );

    // Test line with applyOnCell == false
    alt = vision.GetAltitude( 1, 1, false );
    BOOST_CHECK_CLOSE( 0, alt - refalt, 0.1 );

    // Test no collision with line
    alt = vision.GetAltitude( 1000, 1, true );
    BOOST_CHECK_CLOSE( 0, alt - refalt2, 1.1 );
}
