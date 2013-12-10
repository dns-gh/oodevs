// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/ZipExtractor.h"
#include <tools/Path.h>
#include <tools/TemporaryDirectory.h>

BOOST_AUTO_TEST_CASE( zipstream_extracts_archive )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    auto path = BOOST_RESOLVE( "emptyfile.zip" );
    tools::zipextractor::ExtractArchive( path, dir.Path() );

    auto nonempty = dir.Path() / tools::Path::FromUTF8( "zipdir/nonempty" );
    BOOST_CHECK( nonempty.Exists() );
    auto empty = dir.Path() / tools::Path::FromUTF8( "zipdir/empty" );
    BOOST_CHECK( empty.Exists() );
}

BOOST_AUTO_TEST_CASE( zipstream_lists_package_files )
{
    MOCK_FUNCTOR( f, void( const tools::Path& ) );
    MOCK_EXPECT( f ).once().with( "file1" );
    MOCK_EXPECT( f ).once().with( "data/" );
    MOCK_EXPECT( f ).once().with( "data/file2" );
    MOCK_EXPECT( f ).once().with( "data/subdir/" );
    MOCK_EXPECT( f ).once().with( "data/subdir/file3" );
    tools::zipextractor::ListPackageFiles( BOOST_RESOLVE( "package.zip" ), f );
}

BOOST_AUTO_TEST_CASE( zipstream_installs_package_files )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    const auto& path = dir.Path();
    {
        MOCK_FUNCTOR( f, void() );
        MOCK_EXPECT( f ).exactly( 5 );
        tools::zipextractor::InstallPackageFiles( BOOST_RESOLVE( "package.zip" ), path, f );
    }
    MOCK_FUNCTOR( f, bool( const tools::Path& ) );
    MOCK_EXPECT( f ).once().with( path / "file1" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/file2" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/subdir" ).returns( true );
    MOCK_EXPECT( f ).once().with( path / "data/subdir/file3" ).returns( true );
    BOOST_CHECK_EQUAL( 5u, dir.Path().ListElements( f ).size() );
}

namespace
{
    void ExpectFileContent( tools::zipextractor::InputArchive& a, const tools::Path& p, const std::string& content )
    {
        MOCK_FUNCTOR( f, void( std::string ) );
        MOCK_EXPECT( f ).once().with( content );
        a.ReadPackageFile( p,
            [&]( std::istream& s )
            {
                f( std::string( std::istreambuf_iterator< char >( s ), std::istreambuf_iterator< char >() ) );
            } );
    }
}

BOOST_AUTO_TEST_CASE( zipstream_reads_package_files )
{
    tools::zipextractor::InputArchive a( BOOST_RESOLVE( "package.zip" ) );
    ExpectFileContent( a, "file1", "this is the file content" );
}

BOOST_AUTO_TEST_CASE( zipstream_writes_package_files )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    const auto path = dir.Path() / "package.zip";
    {
        tools::zipextractor::OutputArchive a( path );
        a.WritePackageFile( "file1",
            [&]( std::ostream& s )
            {
                s << "this is the file content";
            } );
        a.WritePackageFile( "data/file2",
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
        tools::zipextractor::ListPackageFiles( path, f );
        BOOST_REQUIRE( f.verify() );
    }
    {
        tools::zipextractor::InputArchive a( path );
        ExpectFileContent( a, "file1", "this is the file content" );
        ExpectFileContent( a, "data/file2", "this is the other file content" );
    }
}
