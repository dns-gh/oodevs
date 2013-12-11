// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/Zip.h"
#include <tools/TemporaryDirectory.h>
#include <tools/StdFileWrapper.h>

namespace
{
    struct InvalidArchiveFixture
    {
        InvalidArchiveFixture()
            : path( BOOST_RESOLVE( "invalid.zip" ) )
        {
            BOOST_REQUIRE( path.Exists() );
        }
        tools::Path path;
    };
}

BOOST_AUTO_TEST_CASE( zipstream_extracts_archive )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    auto path = BOOST_RESOLVE( "emptyfile.zip" );
    tools::zip::ExtractArchive( path, dir.Path() );

    auto nonempty = dir.Path() / tools::Path::FromUTF8( "zipdir/nonempty" );
    BOOST_CHECK( nonempty.Exists() );
    auto empty = dir.Path() / tools::Path::FromUTF8( "zipdir/empty" );
    BOOST_CHECK( empty.Exists() );
}

BOOST_FIXTURE_TEST_CASE( zipstream_extracts_invalid_archive, InvalidArchiveFixture )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    BOOST_CHECK_THROW( tools::zip::ExtractArchive( path, dir.Path() ), std::exception );
}

BOOST_AUTO_TEST_CASE( zipstream_lists_package_files )
{
    MOCK_FUNCTOR( f, void( const tools::Path& ) );
    MOCK_EXPECT( f ).once().with( "file1" );
    MOCK_EXPECT( f ).once().with( "data/" );
    MOCK_EXPECT( f ).once().with( "data/file2" );
    MOCK_EXPECT( f ).once().with( "data/subdir/" );
    MOCK_EXPECT( f ).once().with( "data/subdir/file3" );
    tools::zip::ListPackageFiles( BOOST_RESOLVE( "package.zip" ), f );
}

BOOST_FIXTURE_TEST_CASE( zipstream_lists_invalid_package_files, InvalidArchiveFixture )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    MOCK_FUNCTOR( f, void( const tools::Path& ) );
    BOOST_CHECK_THROW( tools::zip::ListPackageFiles( path, f ), std::exception );
}

BOOST_AUTO_TEST_CASE( zipstream_installs_package_files )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    const auto& path = dir.Path();
    {
        MOCK_FUNCTOR( f, void() );
        MOCK_EXPECT( f ).exactly( 5 );
        tools::zip::InstallPackageFiles( BOOST_RESOLVE( "package.zip" ), path, f );
    }
    MOCK_FUNCTOR( f, bool( const tools::Path& ) );
    MOCK_EXPECT( f ).once().with( path / "file1" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/file2" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/subdir" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/subdir/file3" ).returns( true );
    BOOST_CHECK_EQUAL( 5u, path.ListElements( f ).size() );
}

BOOST_FIXTURE_TEST_CASE( zipstream_installs_invalid_package_files, InvalidArchiveFixture )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    MOCK_FUNCTOR( f, void() );
    BOOST_CHECK_THROW( tools::zip::InstallPackageFiles( path, dir.Path(), f ), std::exception );
}

namespace
{
    void ExpectFileContent( tools::zip::InputArchive& a, const tools::Path& p, const std::string& content )
    {
        MOCK_FUNCTOR( f, void( std::string ) );
        MOCK_EXPECT( f ).once().with( content );
        a.ReadFile( p,
            [&]( std::istream& s )
            {
                f( std::string( std::istreambuf_iterator< char >( s ), std::istreambuf_iterator< char >() ) );
            } );
    }
}

BOOST_AUTO_TEST_CASE( zipstream_reads_package_files )
{
    tools::zip::InputArchive a( BOOST_RESOLVE( "package.zip" ) );
    ExpectFileContent( a, "file1", "this is the file content" );
}

BOOST_AUTO_TEST_CASE( zipstream_reads_package_invalid_file )
{
    tools::zip::InputArchive a( BOOST_RESOLVE( "package.zip" ) );
    MOCK_FUNCTOR( f, void( std::istream& ) );
    BOOST_CHECK_THROW( a.ReadFile( "invalid file", f ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( zipstream_reads_invalid_package_files, InvalidArchiveFixture )
{
    BOOST_CHECK_THROW( tools::zip::InputArchive a( path ), std::exception );
}

BOOST_AUTO_TEST_CASE( zipstream_writes_package_files )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    const auto path = dir.Path() / "package.zip";
    {
        tools::zip::OutputArchive a( path );
        a.WriteFile( "file1",
            [&]( std::ostream& s )
            {
                s << "this is the file content";
            } );
        a.WriteFile( "data/file2",
            [&]( std::ostream& s )
            {
                s << "this is the other file content";
            } );
    }
    {
        MOCK_FUNCTOR( f, void( const tools::Path& ) );
        MOCK_EXPECT( f ).once().with( "file1" );
        //MOCK_EXPECT( f ).once().with( "data/" ); // $$$$ MCO 2013-12-11: how come ? do we care ?
        MOCK_EXPECT( f ).once().with( "data/file2" );
        tools::zip::ListPackageFiles( path, f );
        BOOST_REQUIRE( f.verify() );
    }
    {
        tools::zip::InputArchive a( path );
        ExpectFileContent( a, "file1", "this is the file content" );
        ExpectFileContent( a, "data/file2", "this is the other file content" );
    }
}

BOOST_AUTO_TEST_CASE( zipstream_writes_invalid_package_files )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    BOOST_CHECK_THROW( tools::zip::OutputArchive a( dir.Path() / "***" ), std::exception );
}
