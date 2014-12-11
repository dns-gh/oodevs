// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "reports_plugin/Reports.h"
#include "protocol/Protocol.h"
#include <boost/assign.hpp>
#include <tools/TemporaryDirectory.h>

using namespace plugins::reports;

namespace
{
    sword::Tasker CreateUnitTasker( unsigned int id )
    {
        sword::Tasker tasker;
        tasker.mutable_unit()->set_id( id );
        return tasker;
    }
    sword::Tasker CreateFormationTasker( unsigned int id )
    {
        sword::Tasker tasker;
        tasker.mutable_formation()->set_id( id );
        return tasker;
    }
    sword::Tasker CreateCrowdTasker( unsigned int id )
    {
        sword::Tasker tasker;
        tasker.mutable_crowd()->set_id( id );
        return tasker;
    }
    void FillReport( sword::Report& report, unsigned int id, unsigned int type,
                     sword::Report_EnumReportType reportType, const std::string& date,
                     const sword::Tasker& tasker )
    {
        report.mutable_report()->set_id( id );
        *report.mutable_source() = tasker;
        report.mutable_type()->set_id( type );
        report.set_category( reportType );
        report.mutable_time()->set_data( date );
    }
    void FillParameters( sword::Report& report )
    {
        auto& parameters = *report.mutable_parameters();
        auto& param1 = *parameters.add_elem();
        param1.set_null_value( false );
        param1.add_value()->set_intvalue( 125 );
        auto& param2 = *parameters.add_elem();
        param2.set_null_value( false );
        param2.add_value()->set_areal( 12.5 );
    }
    class Fixture
    {
    public:
        Fixture()
            : tempDir( "testreportsplugin-", testOptions.GetTempDir() )
            , reports( new Reports( tempDir.Path() / "reports.db" ) )
            , minTick( 0 )
            , maxTick( std::numeric_limits< int >::max() )
            , entity1( 1234 )
            , entity2( 12345 )
        {
            FillReport( report1, 542, 0, sword::Report_EnumReportType_information,
                "date1", CreateUnitTasker( entity1 ) );
            FillReport( report2, 541, 1, sword::Report_EnumReportType_operational,
                "date2", CreateFormationTasker( entity2 ) );
            reports->AddReport( report1, 12 );
            reports->AddReport( report2, 12 );
            reports->Commit();

            FillReport( report3, 540, 2, sword::Report_EnumReportType_exceptional_event,
                "date3", CreateCrowdTasker( 123456 ) );
            FillParameters( report3 );
            reports->AddReport( report3, 14 );
            reports->Commit();
        }
    tools::TemporaryDirectory tempDir;
    std::unique_ptr< Reports > reports;
    sword::Report report1;
    sword::Report report2;
    sword::Report report3;
    unsigned int minTick;
    unsigned int maxTick;
    unsigned int entity1;
    unsigned int entity2;
    };
    void CheckReports( const sword::ListReportsAck& ack, const std::vector< sword::Report >& expected,
                       unsigned int nextReport = 0 )
    {
        BOOST_CHECK_EQUAL( ack.reports_size(), static_cast< int >( expected.size() ) );
        for( int i = 0; i < ack.reports_size(); ++i )
            BOOST_CHECK_EQUAL( ack.reports( i ).DebugString(), expected[ i ].DebugString() );
        BOOST_CHECK_EQUAL( ack.has_next_report() ? ack.next_report() : 0u, nextReport );
    }
}

BOOST_FIXTURE_TEST_CASE( list_all_reports, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, 0, std::set< unsigned int >(), boost::none, minTick, 28 );
    CheckReports( ack, std::vector< sword::Report >(), report3.report().id() );

    ack.Clear();
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        std::set< unsigned int >(), boost::none, minTick, maxTick );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report3 )( report2 )( report1 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_one_by_one, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, 1, std::set< unsigned int >(), boost::none, minTick, maxTick );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report3 ), report2.report().id() );

    ack.Clear();
    reports->ListReports( ack, 1, std::set< unsigned int >(), report2.report().id(), minTick, maxTick);
    CheckReports( ack, boost::assign::list_of< sword::Report >( report2 ), report1.report().id() );

    ack.Clear();
    reports->ListReports( ack, 1,std::set< unsigned int >(), report1.report().id(), minTick, maxTick );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report1 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_with_maximum_tick, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        std::set< unsigned int >(), boost::none, minTick, 10 );
    CheckReports( ack, std::vector< sword::Report >() );

    ack.Clear();
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        std::set< unsigned int >(), boost::none, minTick, 13 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report2 )( report1 ) );

    ack.Clear();
    reports->ListReports( ack, 1, std::set< unsigned int >(), boost::none, minTick, 13 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report2 ), report1.report().id() );

    ack.Clear();
    reports->ListReports( ack, 1, std::set< unsigned int >(), report1.report().id(), minTick, 13 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report1 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_with_range_tick, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        std::set< unsigned int >(), boost::none, 13, 13 );
    CheckReports( ack, std::vector< sword::Report >() );

    ack.Clear();
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        std::set< unsigned int >(), boost::none, 13, 14 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report3 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_no_committed, Fixture )
{
    sword::Report report;
    FillReport( report, 539, 0, sword::Report_EnumReportType_information,
        "date6", CreateUnitTasker( 75002 ) );
     reports->AddReport( report, 165 );

     sword::ListReportsAck ack;
     reports->ListReports( ack, std::numeric_limits< int >::max(),
         std::set< unsigned int >(), boost::none, minTick, maxTick );
     CheckReports( ack, boost::assign::list_of< sword::Report >( report )( report3 )( report2 )( report1 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_with_entity_identifier, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        boost::assign::list_of< unsigned int >( 0 ), boost::none, minTick, maxTick );
    CheckReports( ack, std::vector< sword::Report >() );

    ack.Clear();
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        boost::assign::list_of< unsigned int >( entity1 ), boost::none, minTick, maxTick );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report1 ) );

    ack.Clear();
    reports->ListReports( ack, std::numeric_limits< int >::max(),
        boost::assign::list_of< unsigned int >( entity1 )( entity2 ), boost::none, minTick, maxTick );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report2 )( report1 ) );
}
