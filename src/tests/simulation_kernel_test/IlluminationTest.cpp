#include "simulation_kernel_test_pch.h"

#include <set>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include "simulation_kernel/Entities/Agents/Roles/Illumination/PHY_RolePion_Illumination.h"
#include "MockRoleLocation.h"
#include "MockAgent.h"

using namespace mockpp;

namespace
{

    class MockAgentWithPosition : public MockAgent
    {
    public:
        MockAgentWithPosition()
            : MockAgent()
        {
            location = new MockRoleLocation();
            RegisterRole( *location  );
        }

        ~MockAgentWithPosition()
        {
        }

        MockRoleLocation* location;
    };

}

// -----------------------------------------------------------------------------
// Name: IlluminatedByOneTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedByOneTest )
{
    MockAgentWithPosition illuminated;
    MockAgentWithPosition agent;


    const MT_Vector2D pos1;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( illuminated.location ).expects( exactly(2) ).will( returnValue( &pos1 ) );
    const MT_Vector2D pos2;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( agent.location ).expects( exactly(2) ).will( returnValue( &pos2 ) );


    PHY_RolePion_Illumination role( illuminated );
    role.NotifyStartIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated( 2000 ) );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( !role.IsIlluminated( 2000 ) );
}

// -----------------------------------------------------------------------------
// Name: IlluminatedByTwoTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedByTwoTest )
{
    MockAgentWithPosition illuminated;
    MockAgentWithPosition agent;
    MockAgentWithPosition agent2;

    const MT_Vector2D pos1;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( illuminated.location ).expects( exactly(3) ).will( returnValue( &pos1 ) );
    const MT_Vector2D pos2;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( agent.location ).expects( exactly(1) ).will( returnValue( &pos2 ) );
    const MT_Vector2D pos3;
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( agent2.location ).expects( exactly(2) ).will( returnValue( &pos3 ) );


    PHY_RolePion_Illumination role( illuminated );
    role.NotifyStartIlluminatedBy( agent );
    role.NotifyStartIlluminatedBy( agent );//normal double notification to see if agent is added juyt one time
    role.NotifyStartIlluminatedBy( agent2 );
    BOOST_CHECK( role.IsIlluminated( 2000 ) );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated( 2000 ) );
    role.NotifyStopIlluminatedBy( agent2 );
    BOOST_CHECK( !role.IsIlluminated( 2000 ) );
}


// -----------------------------------------------------------------------------
// Name: IlluminatedDefinitelyTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatedDefinitelyTest )
{
    MockAgentWithPosition illuminated;
    MockAgentWithPosition agent;

    PHY_RolePion_Illumination role( illuminated );
    role.NotifyDefinitelyIlluminated();
    BOOST_CHECK( role.IsIlluminated( 2000 ) );
    BOOST_CHECK( role.IsDefinitevelyIlluminated() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( role.IsIlluminated( 2000 ) );
    BOOST_CHECK( role.IsDefinitevelyIlluminated() );
}

// -----------------------------------------------------------------------------
// Name: IlluminatingTest
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IlluminatingTest )
{
    MockAgentWithPosition illuminated;
    MockAgentWithPosition agent;

    PHY_RolePion_Illumination role( illuminated );
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
    MockAgentWithPosition illuminated;
    MockAgentWithPosition agent;

    PHY_RolePion_Illumination role( illuminated );
    role.NotifyStartIlluminatedBy( agent );
    role.NotifyHitByIndirectFire();
    BOOST_CHECK( role.IsUnderIndirectFire() );
    role.NotifyStopIlluminatedBy( agent );
    BOOST_CHECK( !role.IsUnderIndirectFire() );
}