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
    class Fixture
    {
    public:
        Fixture()
            : xis( "<root transfer-time='0h'>"
                   "    <event day='monday' from='22:00' motivation='leisure' to='23:00'/>"
                   "    <event day='friday' from='09:00' motivation='office' to='10:00'/>"
                   "</root>" )
            , schedule( livingArea )
        {
            schedule.Configure( xis >> xml::start( "root" ) );
        }
        xml::xistringstream xis;
        MockMIL_LivingArea livingArea;
        MIL_Schedule schedule;
    };

    unsigned int Convert( unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short sec )
    {
        boost::gregorian::date date( year, month, day );
        boost::posix_time::time_duration time( hour, minute, sec );
        boost::local_time::time_zone_ptr pZone( new boost::local_time::posix_time_zone( "GMT+00:00" ) );
        boost::local_time::local_date_time dateTime( date, time, pZone, false );
        boost::posix_time::ptime epoch( boost::gregorian::date( 1970, 1, 1 ) );
        boost::posix_time::time_duration diff = dateTime.utc_time() - epoch;
        return diff.total_seconds();
    }
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
        MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
        schedule.Update( Convert( 2011, 1, 10, 22, 0, 0 ), 1u );
    }
    MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
    schedule.Update( Convert( 2011, 1, 17, 22, 0, 0 ), 1u );
}

BOOST_FIXTURE_TEST_CASE( tick_duration_schedule_notifies_living_area, Fixture )
{
    MOCK_EXPECT( livingArea, StartMotivation ).once().with( "leisure" );
    schedule.Update( Convert( 2011, 1, 17, 22, 0, 8 ), 10u );
}