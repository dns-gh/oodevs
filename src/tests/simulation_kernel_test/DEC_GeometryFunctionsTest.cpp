// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Decision/DEC_GeometryFunctions.h"
#include "StubTER_World.h"

namespace
{
    struct Fixture
    {
        Fixture()
        {
            WorldInitialize( "worldwide/Paris" );
        }
        ~Fixture()
        {
            TER_World::DestroyWorld();
        }
    };
}

BOOST_FIXTURE_TEST_CASE( split_localisation_cannot_split_when_increment_is_zero, Fixture )
{
    const TER_Localisation localisation( MT_Vector2D( 0, 0 ), 42 );
    TER_Localisation::T_LocalisationPtrVector result =
        DEC_GeometryFunctions::SplitLocalisation( localisation, MT_Vector2D( 0, 0 ), MT_Vector2D( 0, 1 ), 0 );
    BOOST_CHECK( result.empty() );
}

BOOST_FIXTURE_TEST_CASE( split_localisation_cannot_split_when_direction_is_zero, Fixture )
{
    const TER_Localisation localisation( MT_Vector2D( 0, 0 ), 42 );
    TER_Localisation::T_LocalisationPtrVector result =
        DEC_GeometryFunctions::SplitLocalisation( localisation, MT_Vector2D( 0, 0 ), MT_Vector2D( 0, 0 ), 1 );
    BOOST_CHECK( result.empty() );
}

BOOST_FIXTURE_TEST_CASE( split_localisation_wants_a_polygon, Fixture )
{
    T_PointVector points;
    points.push_back( MT_Vector2D( 0, 0 ));
    const TER_Localisation point( TER_Localisation::ePoint, points );
    BOOST_CHECK_THROW( DEC_GeometryFunctions::SplitLocalisation( point, MT_Vector2D( 0, 0 ), MT_Vector2D( 0, 0 ), 1 ), std::runtime_error );
}
