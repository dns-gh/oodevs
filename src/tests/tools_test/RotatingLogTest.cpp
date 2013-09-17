// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/RotatingLog.h"
#include "tools/Log_ABC.h"
#include "tools/LogFactory_ABC.h"
#include <tools/StdFileWrapper.h>
#include <boost/regex.hpp>

using namespace tools;

namespace
{
    MOCK_BASE_CLASS( MockLog, tools::Log_ABC )
    {
        MOCK_METHOD( Write, 1 )
        MOCK_METHOD( Rename, 1 )
        MOCK_METHOD( Delete, 0 )
    };

    MOCK_BASE_CLASS( MockLogFactory, tools::LogFactory_ABC )
    {
        MOCK_METHOD( CreateLog, 2 )
    };

    MOCK_CONSTRAINT( match, regex, boost::regex_match( actual.ToUTF8(), boost::regex( regex ) ) )
}

BOOST_AUTO_TEST_CASE( rotating_log_max_files_set_to_zero_disables_log )
{
    MockLogFactory factory;
    RotatingLog rlog( factory, "filename.log", 0, 42, true );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_max_entries_set_to_zero_disables_rotation )
{
    MockLogFactory factory;
    RotatingLog rlog( factory, "filename.log", 1, 0, true );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).exactly( 3 ).with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_writes_to_log )
{
    MockLogFactory factory;
    RotatingLog rlog( factory, "filename.log", 12, 42, true );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_switches_to_next_log_when_max_size_is_reached )
{
    MockLogFactory factory;
    const unsigned int size = 3;
    RotatingLog rlog( factory, "filename.log", 2, size, true );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( size );
    mock::sequence s;
    MOCK_EXPECT( log->Rename ).once().in( s ).with( match( "filename\\.\\d{8}T\\d{6}\\.log" ) );
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().in( s ).with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_non_initially_empty_log_switches_to_next_log_when_max_size_is_reached )
{
    MockLogFactory factory;
    const unsigned int size = 3, initial = 1;
    RotatingLog rlog( factory, "filename.log", 2, size, true );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", mock::assign( initial ) ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( size - initial );
    MOCK_EXPECT( log->Rename ).once();
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_deletes_oldest_log_when_max_files_is_reached )
{
    MockLogFactory factory;
    const unsigned int size = 3;
    RotatingLog rlog( factory, "filename.log", 2, size, true );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( size );
    MOCK_EXPECT( log->Rename ).once();
    MOCK_EXPECT( log->Delete ).once();
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( size );
    MOCK_EXPECT( log->Rename ).once();
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
}

BOOST_AUTO_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files )
{
    MockLogFactory factory;
    const unsigned int size = 3;
    const tools::Path rotated_1 = "./filename.20130916T115500.log";
    const tools::Path rotated_2 = "./filename.20130916T115501.log";
    tools::Ofstream( rotated_1, std::ios_base::out );
    tools::Ofstream( rotated_2, std::ios_base::out );
    BOOST_REQUIRE( rotated_1.Exists() );
    BOOST_REQUIRE( rotated_2.Exists() );
    MockLog* log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( rotated_1, 0 ).returns( log );
    MOCK_EXPECT( log->Delete ).once();
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( rotated_2, 0 ).returns( log );
    MOCK_EXPECT( log->Delete ).once();
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "./filename.log", 0 ).returns( log );
    RotatingLog rlog( factory, "./filename.log", 2, size, true );
    MOCK_EXPECT( log->Rename ).once();
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 3 );
    log = new MockLog();
    MOCK_EXPECT( factory.CreateLog ).once().with( "./filename.log", 0 ).returns( log );
    MOCK_EXPECT( log->Write ).once().with( "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    rotated_1.Remove();
    rotated_2.Remove();
}
