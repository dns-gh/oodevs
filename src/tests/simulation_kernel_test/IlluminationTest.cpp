#include "simulation_kernel_test_pch.h"

#include <set>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "simulation_kernel/Entities/Agents/Roles/Illumination/PHY_RolePion_Illumination.h"
#include "MockAgent.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: IlluminatedByOneTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedByOneTest )
{
    MockAgent agent;

    PHY_RolePion_Illumination role;
    role.NotifyStartIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( !role.IsIlluminated() );
}

// -----------------------------------------------------------------------------
// Name: IlluminatedByTwoTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedByTwoTest )
{
    MockAgent agent;
    MockAgent agent2;

    PHY_RolePion_Illumination role;
    role.NotifyStartIlluminatedBy( agent );
    role.NotifyStartIlluminatedBy( agent );//normal double notification to see if agent is added juyt one time
    role.NotifyStartIlluminatedBy( agent2 );
    BOOST_CHECK( role.IsIlluminated() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated() );
    role.NotifyStopIlluminatedBy( agent2 );
    BOOST_CHECK( !role.IsIlluminated() );
}


// -----------------------------------------------------------------------------
// Name: IlluminatedDefinitelyTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedDefinitelyTest )
{
    MockAgent agent;

    PHY_RolePion_Illumination role;
    role.NotifyDefinitelyIlluminated();
    BOOST_CHECK( role.IsIlluminated() );
    BOOST_CHECK( role.IsDefinitevelyIlluminated() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated() );
    BOOST_CHECK( role.IsDefinitevelyIlluminated() );
}

// -----------------------------------------------------------------------------
// Name: IlluminatingTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatingTest )
{
    MockAgent agent;

    PHY_RolePion_Illumination role;
    role.NotifyStartIlluminate( agent );
    BOOST_CHECK( role.IsIlluminating() );
    role.NotifyStopIlluminate();
    BOOST_CHECK( !role.IsIlluminating() );
}

// -----------------------------------------------------------------------------
// Name: UnderFireTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( UnderFireTest )
{
    MockAgent agent;

    PHY_RolePion_Illumination role;
    role.NotifyStartIlluminatedBy( agent );
    role.NotifyHitByIndirectFire();
    BOOST_CHECK( role.IsUnderIndirectFire() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( !role.IsUnderIndirectFire() );
}