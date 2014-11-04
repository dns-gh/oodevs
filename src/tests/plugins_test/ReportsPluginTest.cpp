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
        {
            // tick 12
            reports->Update( 12 );
            FillReport( report1, 542, 0, sword::Report_EnumReportType_information,
                "date1", CreateUnitTasker( 1234 ) );
            FillReport( report2, 541, 1, sword::Report_EnumReportType_operational,
                "date2", CreateFormationTasker( 12345 ) );
            reports->AddReport( report1 );
            reports->AddReport( report2 );

            // tick 14
            reports->Update( 14 );
            FillReport( report3, 540, 2, sword::Report_EnumReportType_exceptional_event,
                "date3", CreateCrowdTasker( 123456 ) );
            FillParameters( report3 );
            reports->AddReport( report3 );
        }
    tools::TemporaryDirectory tempDir;
    std::unique_ptr< Reports > reports;
    sword::Report report1;
    sword::Report report2;
    sword::Report report3;
    };
    void CheckReports( const sword::ListReportsAck& ack, const std::vector< sword::Report >& expected,
                       unsigned int nextReport = 0 )
    {
        BOOST_CHECK_EQUAL( ack.reports_size(), expected.size() );
        for( int i = 0; i < ack.reports_size(); ++i )
            BOOST_CHECK_EQUAL( ack.reports( i ).DebugString(), expected[ i ].DebugString() );
        BOOST_CHECK_EQUAL( ack.has_next_report() ? ack.next_report() : 0u, nextReport );
    }
}

BOOST_FIXTURE_TEST_CASE( list_all_reports, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, 123, 0 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report3 )( report2 )( report1 ) );
}

BOOST_FIXTURE_TEST_CASE( list_reports_one_by_one, Fixture )
{
    sword::ListReportsAck ack;
    reports->ListReports( ack, 1, 0 );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report3 ), report2.report().id() );

    ack.Clear();
    reports->ListReports( ack, 1, report2.report().id() );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report2 ), report1.report().id() );

    ack.Clear();
    reports->ListReports( ack, 1, report1.report().id() );
    CheckReports( ack, boost::assign::list_of< sword::Report >( report1 ) );
}
