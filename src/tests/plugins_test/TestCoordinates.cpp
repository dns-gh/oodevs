// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "hla_plugin/Coordinates.h"

using namespace plugins::hla;

namespace
{
    static const double tolerance = 1.; // percent
    static const double sq2     = std::sqrt( 2. );
    static const double pi      = std::acos( -1. );
    static const double piOver2 = pi / 2;
    static const double piOver4 = pi / 4;
}

// -----------------------------------------------------------------------------
// Name: ::Test_CoordinatesOnZero
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_CoordinatesOnZero )
{
    const std::string equatorGreenwich( "31NAA6602100000" );

    WorldLocation location( equatorGreenwich, 0 );

    BOOST_CHECK_CLOSE( 6378137., location.X(), tolerance );
    BOOST_CHECK( std::fabs( location.Y() ) < 1. );
    BOOST_CHECK( std::fabs( location.Z() ) < 1. );

    {
        WorldLocation location( equatorGreenwich, 100 );
        BOOST_CHECK_CLOSE( 6378137. + 100., location.X(), tolerance );
        BOOST_CHECK( std::fabs( location.Y() ) < 1. );
        BOOST_CHECK( std::fabs( location.Z() ) < 1. );
    }

    {
        VelocityVector velocity( location, 10.f, 0 );
        BOOST_CHECK( std::fabs( velocity.X() ) < 0.0001 );
        BOOST_CHECK( std::fabs( velocity.Y() ) < 0.0001 );
        BOOST_CHECK_CLOSE( 1., velocity.Z(), tolerance );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Psi(), tolerance );
        BOOST_CHECK( std::fabs( orientation.Theta() ) < 0.0001 );
        BOOST_CHECK( std::fabs( orientation.Phi() ) < 0.0001 );
    }

    {
        VelocityVector velocity( location, 10.f, 90 );
        BOOST_CHECK( std::fabs( velocity.X() ) < 0.01 );
        BOOST_CHECK_CLOSE( 1., velocity.Y(), tolerance );
        BOOST_CHECK( std::fabs( velocity.Z() ) < 0.0001 );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( piOver2, orientation.Psi(), tolerance );
        BOOST_CHECK( std::fabs( orientation.Theta() ) < 0.0001 );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Phi(), tolerance );
    }

    {
        VelocityVector velocity( location, 10.f, 135 );
        BOOST_CHECK( std::fabs( velocity.X() ) < 0.01 );
        BOOST_CHECK_CLOSE(   sq2 / 2, velocity.Y(), tolerance );
        BOOST_CHECK_CLOSE( - sq2 / 2, velocity.Z(), tolerance );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE(  piOver2, orientation.Psi(), tolerance );
        BOOST_CHECK_CLOSE(  piOver4, orientation.Theta(), tolerance );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Phi(), tolerance );
    }
}

// -----------------------------------------------------------------------------
// Name: ::Test_CoordinatesOn90Zero
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_CoordinatesOn90Zero )
{
    const std::string equator90( "46NAF6602100000" );

    WorldLocation location( equator90, 0 );

    BOOST_CHECK( std::fabs( location.X() ) < 1. );
    BOOST_CHECK_CLOSE( 6378137., location.Y(), tolerance );
    BOOST_CHECK( std::fabs( location.Z() ) < 1. );

    {
        WorldLocation location( equator90, 100 );
        BOOST_CHECK( std::fabs( location.X() ) < 1. );
        BOOST_CHECK_CLOSE( 6378137. + 100., location.Y(), tolerance );
        BOOST_CHECK( std::fabs( location.Z() ) < 1. );
    }

    {
        VelocityVector velocity( location, 10.f, 0 );
        BOOST_CHECK( std::fabs( velocity.X() ) < 0.0001 );
        BOOST_CHECK( std::fabs( velocity.Y() ) < 0.0001 );
        BOOST_CHECK_CLOSE( 1., velocity.Z(), tolerance );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Psi(), tolerance );
        BOOST_CHECK( std::fabs( orientation.Theta() ) < 0.0001 );
        BOOST_CHECK( std::fabs( orientation.Phi() ) < 0.0001 );
    }

    {
        VelocityVector velocity( location, 10.f, 90 );
        BOOST_CHECK_CLOSE( -1., velocity.X(), tolerance );
        BOOST_CHECK( std::fabs( velocity.Y() ) < 0.0001 );
        BOOST_CHECK( std::fabs( velocity.Z() ) < 0.0001 );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( pi, orientation.Psi(), tolerance );
        BOOST_CHECK( std::fabs( orientation.Theta() ) < 0.0001 );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Phi(), tolerance );
    }

    {
        VelocityVector velocity( location, 10.f, 135 );
        BOOST_CHECK_CLOSE( - sq2 / 2, velocity.X(), tolerance );
        BOOST_CHECK( std::fabs( velocity.Y() ) < 0.01 );
        BOOST_CHECK_CLOSE( - sq2 / 2, velocity.Z(), tolerance );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE(  pi, orientation.Psi(), tolerance );
        BOOST_CHECK_CLOSE( piOver4, orientation.Theta(), tolerance );
        BOOST_CHECK_CLOSE( -piOver2, orientation.Phi(), tolerance );
    }
}

// -----------------------------------------------------------------------------
// Name: ::Test_CoordinatesOn90Zero
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_CoordinatesOnZero45 )
{
    const std::string greenwich45( "31TBK6355487330" );

    WorldLocation location( greenwich45, 0 );

    // Ellipsoidities
    BOOST_CHECK_CLOSE( 4517590., location.X(), tolerance );
    BOOST_CHECK( std::fabs( location.Y() ) < 1. );
    BOOST_CHECK_CLOSE( 4487348., location.Z(), tolerance );

    {
        WorldLocation location( greenwich45, 100 );
        BOOST_CHECK_CLOSE( 4517590. + 100. * sq2 / 2, location.X(), tolerance );
        BOOST_CHECK( std::fabs( location.Y() ) < 1. );
        BOOST_CHECK_CLOSE( 4487348. + 100. * sq2 / 2, location.Z(), tolerance );
    }

    {
        VelocityVector velocity( location, 10.f, 0 );
        BOOST_CHECK_CLOSE( -sq2/2., velocity.X(), tolerance );
        BOOST_CHECK( std::fabs( velocity.Y() ) < 0.0001 );
        BOOST_CHECK_CLOSE(  sq2/2., velocity.Z(), tolerance );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( -pi, orientation.Psi(), tolerance );
        BOOST_CHECK_CLOSE( -piOver4, orientation.Theta(), tolerance );
        BOOST_CHECK_CLOSE( pi, orientation.Phi(), tolerance );
    }

    {
        VelocityVector velocity( location, 10.f, 90 );
        BOOST_CHECK( std::fabs( velocity.X() ) < 0.01 );
        BOOST_CHECK_CLOSE( 1., velocity.Y(), tolerance );
        BOOST_CHECK( std::fabs( velocity.Z() ) < 0.0001 );

        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE( piOver2, orientation.Psi(), tolerance );
        BOOST_CHECK( std::fabs( orientation.Theta() ) < 0.0001 );
        BOOST_CHECK_CLOSE( -piOver2-piOver4, orientation.Phi(), tolerance );
    }

    {
        VelocityVector velocity( location, 10.f, 135 );
        BOOST_CHECK_CLOSE(   0.5,     velocity.X(), tolerance );
        BOOST_CHECK_CLOSE(   sq2 / 2, velocity.Y(), tolerance );
        BOOST_CHECK_CLOSE( - 0.5,     velocity.Z(), tolerance );

        // whatever
        Orientation orientation( location, velocity );
        BOOST_CHECK_CLOSE(  0.9569, orientation.Psi(), tolerance ); 
        BOOST_CHECK_CLOSE( 0.5255, orientation.Theta(), tolerance );
        BOOST_CHECK_CLOSE( -2.522, orientation.Phi(), tolerance );
    }
}
