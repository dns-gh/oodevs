// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "host_test_pch.h"
#include "host/Error.h"
#include "MockFileSystem.h"
#include <boost/bind/apply.hpp>

using namespace mocks;

namespace
{
    std::string MakeInfo( const std::string& m )
    {
        return "[2013-11-04 11:06:06] <Simulation> <info> " + m + "\n";
    }
    std::string MakeError( const std::string& m )
    {
        return "[2013-11-04 11:06:06] <Simulation> <functERR> " + m + "\n";
    }
    const std::string pre = MakeInfo( "some info before" );
    const std::string post = MakeInfo( "some info after" );

    const runtime::Path path = "some\\funky //\\\\path";

    void PopulateLog( const std::function< bool( const runtime::Path& ) >& f )
    {
        f( path / "Sim.log" );
    }
}

BOOST_AUTO_TEST_CASE( last_error_of_non_existing_or_empty_log_is_empty )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( "" );
    BOOST_CHECK( host::GetLastError( fs, path ).empty() );
}

BOOST_AUTO_TEST_CASE( last_error_of_log_with_no_error_is_empty )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( MakeInfo( "no error to report" ) );
    BOOST_CHECK( host::GetLastError( fs, path ).empty() );
}

BOOST_AUTO_TEST_CASE( last_error_extracts_error_from_end_of_log )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( pre + MakeError( "the error message" ) );
    BOOST_CHECK_EQUAL( "the error message\n", host::GetLastError( fs, path ) );
}

BOOST_AUTO_TEST_CASE( last_error_extracts_error_from_middle_of_log )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( pre + MakeError( "the error message" ) + post );
    BOOST_CHECK_EQUAL( "the error message\n", host::GetLastError( fs, path ) );
}

BOOST_AUTO_TEST_CASE( last_error_extracts_multi_line_error_from_middle_of_log )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( pre + MakeError( "the \n error \n message" ) + post );
    BOOST_CHECK_EQUAL( "the \n error \n message\n", host::GetLastError( fs, path ) );
}

BOOST_AUTO_TEST_CASE( last_error_extracts_error_from_consecutive_errors )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLog, _3 ) );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( MakeError( "the previous error message" ) + MakeError( "the error message" ) );
    BOOST_CHECK_EQUAL( "the error message\n", host::GetLastError( fs, path ) );
}

namespace
{
    bool PopulateLogs( const std::function< bool( const runtime::Path& ) >& f )
    {
        f( path / "Sim.20101104T110600.log" );
        f( path / "Sim.20131104T110600.log" );
        f( path / "Sim.log" );
        f( path / "Sim.20111104T110600.log" );
        return true;
    }
}

BOOST_AUTO_TEST_CASE( last_error_extracts_error_from_rotated_log )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLogs, _3 ) );
    mock::sequence s;
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).in( s ).returns( pre + MakeInfo( "no error in this one" ) + post );
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.20131104T110600.log" ).in( s ).returns( MakeError( "the error message" ) );
    BOOST_CHECK_EQUAL( "the error message\n", host::GetLastError( fs, path ) );
}

BOOST_AUTO_TEST_CASE( last_error_stops_looking_in_rotated_logs_when_finding_start_banner )
{
    MockFileSystem fs;
    MOCK_EXPECT( fs.Walk ).with( path, false, mock::any ).calls( boost::bind( &PopulateLogs, _3 ) );
    const std::string banner = "[2013-11-04 11:06:00] <Simulation> <info> Sword Simulation - Version 5.3.0.22158 - Release\n";
    MOCK_EXPECT( fs.ReadFile ).with( path / "Sim.log" ).returns( banner + pre + MakeInfo( "no error in this one" ) + post );
    BOOST_CHECK( host::GetLastError( fs, path ).empty() );
}
