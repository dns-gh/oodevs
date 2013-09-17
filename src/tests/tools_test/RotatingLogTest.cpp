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
#include "tools/LogFactory_ABC.h"
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>
#include <boost/regex.hpp>

using namespace tools;

namespace
{
    MOCK_BASE_CLASS( MockLogFactory, tools::LogFactory_ABC )
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
        {}
        tools::TemporaryDirectory dir;
        tools::Path filename;
        MockLogFactory factory;
        const unsigned int size;
    };
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_files_set_to_zero_disables_log, Fixture )
{
    RotatingLog rlog( factory, filename, 0, 42, true );
    rlog.Write( "some text" );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_max_entries_set_to_zero_disables_rotation, Fixture )
{
    RotatingLog rlog( factory, filename, 1, 0, true );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).exactly( 3 ).with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_writes_to_log, Fixture )
{
    RotatingLog rlog( factory, filename, 12, 42, true );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_switches_to_next_log_when_max_size_is_reached, Fixture )
{
    RotatingLog rlog( factory, filename, 2, size, true );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( size );
    mock::sequence s;
    MOCK_EXPECT( factory.ComputeSize ).once().in( s ).with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_FUNCTOR( f, bool( const Path& ) );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( f );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_non_initially_empty_log_switches_to_next_log_when_max_size_is_reached, Fixture )
{
    const unsigned int initial = 1;
    RotatingLog rlog( factory, filename, 2, size, true );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( initial );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( size - initial );
    MOCK_EXPECT( factory.ComputeSize ).once().returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    MOCK_FUNCTOR( f, bool( const Path& ) );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( f );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_deletes_oldest_log_when_max_files_is_reached, Fixture )
{
    RotatingLog rlog( factory, filename, 2, size, true );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( size );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( size );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.Write ).once().with( mock::any, "some text" ).returns( 0 );
    rlog.Write( "some text" );
    rlog.Write( "some text" );
    tools::Path path;
    MOCK_FUNCTOR( f, bool( const Path& ) );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( f );
    rlog.Write( "some text" );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( f );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_truncating_current_log_file, Fixture )
{
    const tools::Path rotated_1( dir.Path() / "filename.20130916T115500.log" );
    const tools::Path rotated_2( dir.Path() / "filename.20130916T115501.log" );
    tools::Ofstream( rotated_1, std::ios_base::out );
    tools::Ofstream( rotated_2, std::ios_base::out );
    BOOST_REQUIRE( rotated_1.Exists() );
    BOOST_REQUIRE( rotated_2.Exists() );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( factory, filename, 3, size, true );
    MOCK_FUNCTOR( f, bool( const Path& ) );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( rotated_2 ).returns( false );
    MOCK_EXPECT( f ).once().with( match( boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) ).returns( false );
    dir.Path().ListElements( f );
}

BOOST_FIXTURE_TEST_CASE( rotating_log_re_lists_existing_rotated_log_files_appending_to_current_log_file, Fixture )
{
    const tools::Path rotated_1( dir.Path() / "filename.20130916T115500.log" );
    const tools::Path rotated_2( dir.Path() / "filename.20130916T115501.log" );
    tools::Ofstream( rotated_1, std::ios_base::out );
    tools::Ofstream( rotated_2, std::ios_base::out );
    BOOST_REQUIRE( rotated_1.Exists() );
    BOOST_REQUIRE( rotated_2.Exists() );
    MOCK_EXPECT( factory.ComputeSize ).once().with( filename ).returns( 0 );
    RotatingLog rlog( factory, filename, 3, size, false );
    MOCK_FUNCTOR( f, bool( const Path& ) );
    MOCK_EXPECT( f ).once().with( filename ).returns( false );
    MOCK_EXPECT( f ).once().with( rotated_1 ).returns( false );
    MOCK_EXPECT( f ).once().with( rotated_2 ).returns( false );
    dir.Path().ListElements( f );
}
