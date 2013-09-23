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
}

BOOST_AUTO_TEST_CASE( rotating_log_does_not_get_confused_by_escaped_characters_in_path )
{
    MockLog log;
    MOCK_EXPECT( log.ComputeSize ).returns( 0 );
    RotatingLog rlog( log, "SWORD\\exercises\\Egypt\\sessions\\default\\Protobuf\\some.log", 0, 3, true );
}

namespace
{
    struct Fixture
    {
        Fixture()
            : dir( "rotation_log_test_", temp_directory )
            , filename( dir.Path() / "filename.log" )
        {}
        tools::TemporaryDirectory dir;
        tools::Path filename;
        MockLog log;
        MOCK_FUNCTOR( validator, bool( const Path& ) );
    };

    MOCK_CONSTRAINT( match, regex, boost::regex_match( actual.ToUTF8(), boost::regex( regex ) ) )
    const auto created = match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_files_set_to_zero_disables_log, Fixture )
{
    MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( log, filename, 0, 3, true );
    rlog.Write( "some text" );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_size_set_to_zero_disables_rotation, Fixture )
{
    MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( log, filename, 2, 0, true );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 3 );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_switches_to_next_log_after_max_size_is_reached, Fixture )
{
    MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( log, filename, 2, 3, true );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 3 );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( created ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_non_initially_empty_switches_to_next_log_when_max_size_is_reached, Fixture )
{
    const unsigned int initial = 1;
    MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( initial );
    RotatingLog rlog( log, filename, 2, 3, true );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 3 - initial );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( created ).returns( false );
    dir.Path().ListElements( validator );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_deletes_oldest_log_when_max_files_is_reached, Fixture )
{
    MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( log, filename, 2, 3, true );
    MOCK_EXPECT( log.Write ).exactly( 2 ).with( mock::any, "some text" ).returns( 3 );
    MOCK_EXPECT( log.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    tools::Path path;
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( created && mock::retrieve( path ) ).returns( false );
    dir.Path().ListElements( validator );
    rlog.Write( "some text" );
    MOCK_EXPECT( validator ).once().with( filename ).returns( false );
    MOCK_EXPECT( validator ).once().with( created && ! mock::equal( path ) ).returns( false );
    dir.Path().ListElements( validator );
}

namespace
{
    struct ReListFixture : Fixture
    {
        ReListFixture()
            : rotated_1( dir.Path() / "filename.20130916T115500.log" )
            , rotated_2( dir.Path() / "filename.20130916T115501.log" )
            , unrelated_1( dir.Path() / "unrelated_1.20130916T115503.log" )
            , unrelated_2( dir.Path() / "unrelated_filename.20130916T115503.log" )
            , unrelated_3( dir.Path() / "filename.20130916T115503.log_unrelated" )
        {}

        void Run( boost::optional< const char* > content, std::size_t files, std::size_t size, bool truncate )
        {
            dir.Path().CreateDirectories();
            tools::Ofstream( rotated_1, std::ios_base::out );
            tools::Ofstream( rotated_2, std::ios_base::out );
            tools::Ofstream( unrelated_1, std::ios_base::out );
            tools::Ofstream( unrelated_2, std::ios_base::out );
            tools::Ofstream( unrelated_3, std::ios_base::out );
            if( content )
                tools::Ofstream( filename, std::ios_base::out ) << *content;
            MOCK_EXPECT( log.ComputeSize ).once().with( filename ).returns( content ? 1 : 0 );
            RotatingLog rlog( log, filename, files, size, truncate );
            dir.Path().ListElements( validator );
            dir.Path().RemoveAll();
            MOCK_VERIFY( validator );
            MOCK_RESET( validator );
        }
#define EXPECT(z, n, d) \
        MOCK_EXPECT( validator ).once().with( t##n ).returns( false );
#define CHECK(z, n, d) \
        template< typename C BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T) > \
        void Check( C content, std::size_t files, std::size_t size, bool truncate \
            BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T, &t) ) \
        { \
            MOCK_EXPECT( validator ).once().with( unrelated_1 ).returns( false ); \
            MOCK_EXPECT( validator ).once().with( unrelated_2 ).returns( false ); \
            MOCK_EXPECT( validator ).once().with( unrelated_3 ).returns( false ); \
            BOOST_PP_REPEAT(n, EXPECT, _) \
            Run( content, files, size, truncate ); \
        }
        BOOST_PP_REPEAT(4, CHECK, _)

        const tools::Path rotated_1, rotated_2;
        const tools::Path unrelated_1, unrelated_2, unrelated_3;
    };
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files, ReListFixture )
{
    Check( "initial content", 3, 3, true, rotated_2, created );
    Check( "initial content", 3, 3, false, rotated_1, rotated_2, filename );
    Check( "initial content", 2, 3, true, created );
    Check( "initial content", 2, 3, false, rotated_2, filename );
    Check( "initial content", 1, 3, true );
    Check( "initial content", 1, 3, false, filename );
    Check( "initial content", 0, 3, true );
    Check( "initial content", 0, 3, false, filename );
    Check( boost::none, 3, 3, true, rotated_2, created );
    Check( boost::none, 3, 3, false, rotated_1, rotated_2 );
    Check( boost::none, 2, 3, true, created );
    Check( boost::none, 2, 3, false, rotated_2 );
    Check( boost::none, 1, 3, true );
    Check( boost::none, 1, 3, false );
    Check( boost::none, 0, 3, true );
    Check( boost::none, 0, 3, false );
}
