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
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>
#include <boost/regex.hpp>

using namespace tools;

namespace
{
    MOCK_BASE_CLASS( MockLog, tools::Log_ABC )
    {
        MOCK_METHOD( Write, 2 )
        MOCK_METHOD( ComputeSize, 1 )
    };

    MOCK_CONSTRAINT( match, regex, boost::regex_match( actual.ToUTF8(), boost::regex( regex ) ) )

    struct Fixture
    {
        Fixture()
            : dir( "rotation_log_test_", temp_directory )
            , filename( dir.Path() / "filename.log" )
            , size( 3 )
        {
            MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( 0 );
        }
        tools::TemporaryDirectory dir;
        tools::Path filename;
        MockLog log;
        const unsigned int size;
        MOCK_FUNCTOR( validator, bool( const Path& ) );
    };
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_files_set_to_zero_disables_log, Fixture )
{
    RotatingLog rlog( log, filename, 0, 42, true );
    rlog.Write( "some text" );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_entries_set_to_zero_disables_rotation, Fixture )
{
    RotatingLog rlog( log, filename, 1, 0, true );
    MOCK_EXPECT( log.Write ).exactly( 3 ).with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_switches_to_next_log_when_max_size_is_reached, Fixture )
{
    RotatingLog rlog( log, filename, 2, size, true );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( size );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_non_initially_empty_log_switches_to_next_log_when_max_size_is_reached, Fixture )
{
    const unsigned int initial = 1;
    RotatingLog rlog( log, filename, 2, size, true );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( size - initial );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_deletes_oldest_log_when_max_files_is_reached, Fixture )
{
    RotatingLog rlog( log, filename, 2, size, true );
    MOCK_EXPECT( log.Write ).exactly( 2 ).with( mock::any, "some text" ).returns( size );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    tools::Path path;
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
}

namespace
{
    struct ReListFixture : Fixture
    {
        ReListFixture()
            : rotated_1( dir.Path() / "filename.20130916T115500.log" )
            , rotated_2( dir.Path() / "filename.20130916T115501.log" )
            , unrelated( dir.Path() / "unrelated.20130916T115503.log" )
        {
            tools::Ofstream( rotated_1, std::ios_base::out );
            tools::Ofstream( rotated_2, std::ios_base::out );
            tools::Ofstream( unrelated, std::ios_base::out );
            BOOST_REQUIRE( rotated_1.Exists() );
            BOOST_REQUIRE( rotated_2.Exists() );
            BOOST_REQUIRE( unrelated.Exists() );
        }
        const tools::Path rotated_1, rotated_2, unrelated;
    };
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_truncating_current_log_file, ReListFixture )
{
    RotatingLog rlog( log, filename, 3, size, true );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( rotated_2 ).returns( false );
    MOCK_EXPECT( validator ).once().with( unrelated ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_appending_to_current_log_file, ReListFixture )
{
    RotatingLog rlog( log, filename, 3, size, false );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( rotated_1 ).returns( false );
    MOCK_EXPECT( validator ).once().with( rotated_2 ).returns( false );
    MOCK_EXPECT( validator ).once().with( unrelated ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_deleting_oldest_files_and_truncating_current_log_file, ReListFixture )
{
    RotatingLog rlog( log, filename, 3, size, true );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( rotated_2 ).returns( false );
    MOCK_EXPECT( validator ).once().with( unrelated ).returns( false );
    MOCK_EXPECT( validator ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_deleting_oldest_files_and_appending_to_current_log_file, ReListFixture )
{
    RotatingLog rlog( log, filename, 2, size, false );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( rotated_2 ).returns( false );
    MOCK_EXPECT( validator ).once().with( unrelated ).returns( false );
    dir.Path().ListElements( validator );
}
