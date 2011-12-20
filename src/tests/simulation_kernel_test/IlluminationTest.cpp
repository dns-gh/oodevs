#include "simulation_kernel_test_pch.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "simulation_kernel/Entities/Agents/Roles/Illumination/PHY_RolePion_Illumination.h"
#include "MockRoleLocation.h"
#include "MockAgent.h"

namespace
{
    class MockAgentWithPosition : public MockAgent
    {
    public:
        MockAgentWithPosition()
        {
            RegisterRole( *new MockRoleLocation() );
        }
        virtual ~MockAgentWithPosition()
        {
            // NOTHING
        }
    };
}

// -----------------------------------------------------------------------------
// Name: IlluminatedByOneTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedByOneTest )
{
    MockAgentWithPosition agent;

    const MT_Vector2D pos1;
    const MT_Vector2D pos2;

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
    MockAgentWithPosition agent;
    MockAgentWithPosition agent2;

    const MT_Vector2D pos1;
    const MT_Vector2D pos2;
    const MT_Vector2D pos3;

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
    MockAgentWithPosition agent;

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
    MockAgentWithPosition agent;

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
    MockAgentWithPosition agent;

    PHY_RolePion_Illumination role;
    role.NotifyStartIlluminatedBy( agent );
    role.NotifyHitByIndirectFire();
    BOOST_CHECK( role.IsUnderIndirectFire() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( !role.IsUnderIndirectFire() );
}