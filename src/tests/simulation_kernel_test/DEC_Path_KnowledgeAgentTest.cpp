// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Decision/DEC_Path_KnowledgeAgent.h"
#include <boost/math/constants/constants.hpp>

namespace
{
    double ComputeCost( double maxRangeToFire, double distanceFromEnemy, double enemyCostAtSecurityRange, double enemyCostOnContact )
    {
        const MT_Vector2D enemyPosition( 0, distanceFromEnemy );
        const DEC_Path_KnowledgeAgent cost( enemyPosition, enemyCostAtSecurityRange, enemyCostOnContact, maxRangeToFire );
        const MT_Vector2D from( 0, 0 );
        const MT_Vector2D to( 1000, 0 );
        const MT_Line lineLink( from, to );
        const DEC_Path_KnowledgeAgent::BoundingBox box( from, to );
        return cost.ComputeCost( lineLink, box );
    }
}

BOOST_AUTO_TEST_CASE( enemy_max_range_to_fire_is_minimum_1000_meters )
{
    const double enemyCostAtSecurityRange = 10;
    const double enemyCostOnContact = 10;
    const double closeEnemy = 0;
    BOOST_CHECK( ComputeCost( 0, closeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ) > 0.1 );
    BOOST_CHECK_CLOSE( ComputeCost( 0, closeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ), ComputeCost( 999, closeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ), 0.0001 );
}

BOOST_AUTO_TEST_CASE( enemy_max_range_to_fire_is_maximum_2000_meters )
{
    const double enemyCostAtSecurityRange = 10;
    const double enemyCostOnContact = 10;
    const double limitRangeEnemy = 1999;
    BOOST_CHECK( ComputeCost( 2001, limitRangeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ) > 0.1 );
    BOOST_CHECK_CLOSE( ComputeCost( 2001, limitRangeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ), ComputeCost( 3000, limitRangeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ), 0.0001 );
}

BOOST_AUTO_TEST_CASE( enemy_to_far_from_segment_returns_empty_cost )
{
    const double enemyCostAtSecurityRange = 10;
    const double enemyCostOnContact = 10;
    const double rangeToFire = 2000;
    BOOST_CHECK_CLOSE( ComputeCost( rangeToFire, 4000, enemyCostAtSecurityRange, enemyCostOnContact ), 0., 0.0001 );
}

BOOST_AUTO_TEST_CASE( enemy_outside_range_to_fire_returns_empty_cost )
{
    const double enemyCostAtSecurityRange = 10;
    const double enemyCostOnContact = 10;
    const double rangeToFire = 1000;
    BOOST_CHECK_CLOSE( ComputeCost( rangeToFire, rangeToFire + 10, enemyCostAtSecurityRange, enemyCostOnContact ), 0., 0.0001 );
}

BOOST_AUTO_TEST_CASE( empty_contact_cost_returns_empty_cost )
{
    const double enemyCostAtSecurityRange = 10;
    const double rangeToFire = 1000;
    const double closeEnemy = 0;
    BOOST_CHECK_CLOSE( ComputeCost( rangeToFire, closeEnemy, enemyCostAtSecurityRange, 0 ), 0., 0.0001 );
}

BOOST_AUTO_TEST_CASE( cost_is_an_invert_square_law )
{
    const double enemyCostAtSecurityRange = 1;
    const double rangeToFire = 1000;
    const double enemyCostOnContact = 100;
    const double closeEnemy = 1;
    const double expected = rangeToFire * rangeToFire / ( boost::math::constants::pi< double >() + 1 );
    BOOST_CHECK_CLOSE( ComputeCost( rangeToFire, closeEnemy, enemyCostAtSecurityRange, enemyCostOnContact ), expected, 0.0001 );
}
