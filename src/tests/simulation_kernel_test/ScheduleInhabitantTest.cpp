// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockMIL_LivingArea.h"
#include "Entities/Inhabitants/MIL_Schedule.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( disable : 4244 )
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

BOOST_AUTO_TEST_CASE( no_events_does_nothing )
{
    xml::xistringstream xis( "<root transfer-time='0h'/>" );
    MockMIL_LivingArea livingArea;
    MIL_Schedule schedule( livingArea );
    schedule.Configure( xis >> xml::start( "root" ) );
    schedule.Update( 1u, 1u );
}

BOOST_AUTO_TEST_CASE( wrong_day_in_xml_throws )
{
    xml::xistringstream xis( "<root transfer-time='0h'>"
                             "    <event day='error' from='22:00' motivation='leisure' to='23:00'/>"
                             "</root>" );
    MockMIL_LivingArea livingArea;
    MIL_Schedule schedule( livingArea );
    BOOST_CHECK_THROW( schedule.Configure( xis >> xml::start( "root" ) ), std::runtime_error );
}

namespace
{
    class InitFixture
    {
    public:
        InitFixture()
            : xis( "<root transfer-time='40m'>"
                   "    <event day='monday' from='22:00' motivation='leisure' to='23:00'/>"
                   "    <event day='friday' from='09:00' motivation='office' to='10:00'/>"
                   "</root>" )
            , schedule( livingArea )
        {
            mock::reset();
            xis >> xml::start( "root" );
        }
        xml::xistringstream xis;
        MockMIL_LivingArea livingArea;
        MIL_Schedule schedule;
    };

    unsigned int Convert( unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short sec )
    {
        boost::posix_time::ptime now( boost::gregorian::date( year, month, day ), boost::posix_time::time_duration( hour, minute, sec ) );
        boost::posix_time::ptime epoch( boost::gregorian::date( 1970, 1, 1 ) );
        return ( now - epoch ).total_seconds();
    }
}

BOOST_FIXTURE_TEST_CASE( population_initialize_his_motivation, InitFixture )
{
    {
        MIL_Schedule schedule( livingArea );
        schedule.Configure( xis );
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        MOCK_EXPECT( livingArea, FinishMoving ).once();
        schedule.Update( Convert( 2011, 3, 22, 11, 15, 0 ), 1u );
        mock::verify();
    }
    {
        MIL_Schedule schedule( livingArea );
        schedule.Configure( xis );
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        MOCK_EXPECT( livingArea, FinishMoving ).once();
        schedule.Update( Convert( 2011, 3, 24, 11, 15, 0 ), 1u );
        mock::verify();
    }
    {
        MIL_Schedule schedule( livingArea );
        schedule.Configure( xis );
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "office" );
        MOCK_EXPECT( livingArea, FinishMoving ).once();
        schedule.Update( Convert( 2011, 3, 26, 11, 15, 0 ), 1u );
        mock::verify();
    }
    {
        MIL_Schedule schedule( livingArea );
        schedule.Configure( xis );
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "office" );
        MOCK_EXPECT( livingArea, FinishMoving ).once();
        schedule.Update( Convert( 2011, 3, 28, 11, 15, 0 ), 1u );
        mock::verify();
    }
}

namespace
{
    class Fixture : public InitFixture
    {
    public:
        Fixture()
            : schedule( livingArea )
        {
            schedule.Configure( xis );
            MOCK_EXPECT( livingArea, StartMotivation ).once().with( "office" );
            MOCK_EXPECT( livingArea, FinishMoving ).once();
            schedule.Update( Convert( 2011, 1, 3, 10, 10, 0 ), 1u );
            mock::verify();
        }
        MIL_Schedule schedule;
    };
}

BOOST_FIXTURE_TEST_CASE( wrong_time_does_nothing, Fixture )
{
    schedule.Update( Convert( 2011, 1, 3, 11, 15, 0 ), 1u );
}

BOOST_FIXTURE_TEST_CASE( wrong_day_does_nothing, Fixture )
{
    schedule.Update( Convert( 2011, 1, 4, 22, 0, 0 ), 1u );
}

BOOST_FIXTURE_TEST_CASE( wrong_duration_does_nothing, Fixture )
{
    schedule.Update( Convert( 2011, 1, 17, 22, 0, 8 ), 5u );
}

BOOST_FIXTURE_TEST_CASE( schedule_notifies_living_area, Fixture )
{
    {
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        schedule.Update( Convert( 2011, 1, 17, 22, 0, 0 ), 1u );
    }
    {
        MOCK_EXPECT( livingArea, MovePeople ).once().with( "leisure", 3 );
        schedule.Update( Convert( 2011, 1, 17, 22, 15, 0 ), 1u );
    }
    {
        MOCK_EXPECT( livingArea, MovePeople ).once().with( "leisure", 3 );
        schedule.Update( Convert( 2011, 1, 17, 22, 30, 0 ), 1u );
    }
    {
        MOCK_EXPECT( livingArea, FinishMoving ).once().with( "leisure" );
        schedule.Update( Convert( 2011, 1, 17, 22, 40, 0 ), 1u );
    }
}

BOOST_FIXTURE_TEST_CASE( schedule_time_magic_action_notifies_living_area, Fixture )
{
    {
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        schedule.Update( Convert( 2011, 1, 17, 22, 0, 0 ), 1u );
    }
    MOCK_EXPECT( livingArea, StartMotivation ).once().with( "office" );
    schedule.Update( Convert( 2011, 1, 21, 9, 0, 0 ), 1u );
}

BOOST_FIXTURE_TEST_CASE( every_week_schedule_notifies_living_area, Fixture )
{
    {
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        schedule.Update( Convert( 2011, 1, 3, 22, 0, 0 ), 1u );
    }
    {
        MOCK_EXPECT( livingArea, FinishMoving ).with( "leisure" );
        schedule.Update( Convert( 2011, 1, 3, 22, 40, 0 ), 1u );
    }
    MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
    schedule.Update( Convert( 2011, 1, 17, 22, 0, 0 ), 1u );
}

BOOST_FIXTURE_TEST_CASE( tick_duration_schedule_notifies_living_area, Fixture )
{
    MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
    schedule.Update( Convert( 2011, 1, 17, 22, 0, 8 ), 10u );
}
