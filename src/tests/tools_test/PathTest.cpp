// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/FileWrapper.h"
#include "tools/Path.h"
#include <tools/TemporaryDirectory.h>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <windows.h>
#include <tchar.h>

namespace
{
    void CreateFile( const tools::Path& file )
    {
        if( file.IsDirectory() )
            return;
        tools::Ofstream os( file );
        os.close();
    }

    void PopulateSampleDirectory( const tools::Path& directory, const tools::Path& baseFileName = "test", unsigned int numberOfFiles = 10 )
    {
        if( !directory.IsDirectory() || !directory.Exists() )
            return;
        for( unsigned int i = 0; i < numberOfFiles; ++i )
        {
            tools::Path child = directory / baseFileName + tools::Path::FromUTF8( boost::lexical_cast< std::string >( i ) ) + ".txt";
            CreateFile( child );
        }
    }

    bool CheckFileName( const std::wstring& name, const tools::Path& file )
    {
        return file.BaseName().ToUnicode().find( name ) != std::wstring::npos;
    }

    std::wstring GetTempPath()
    {
        DWORD result = ::GetTempPathW( 0, L"" );
        if( result == 0 )
            throw std::runtime_error( "Could not get system temp path" );

        std::vector< WCHAR > tempPath( result );

        result = ::GetTempPathW( static_cast< DWORD >( tempPath.size() ), &tempPath[0] );
        if( result == 0 || result > tempPath.size() )
            throw std::runtime_error( "Could not get system temp path" );

        return std::wstring( tempPath.begin(), tempPath.begin() + static_cast< std::size_t >( result ) );
    }

    std::wstring GetCurrentPath()
    {
        WCHAR currentDirectory[ MAX_PATH ];
        GetCurrentDirectoryW( MAX_PATH, currentDirectory );
        return currentDirectory;
    }
}

// -----------------------------------------------------------------------------
// Decomposition
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_decomposition )
{
    const tools::Path base = "C:/foo/bar.txt";
    const tools::Path root = base.Root();                   // C:/foo/bar.txt   =>   C:/
    const tools::Path parent = base.Parent();               // C:/foo/bar.txt   =>   C:/foo
    const tools::Path filename = base.FileName();           // C:/foo/bar.txt   =>   bar.txt
    const tools::Path basename = base.BaseName();           // C:/foo/bar.txt   =>   bar
    const tools::Path extension = base.Extension();         // C:/foo/bar.txt   =>   .txt
    tools::Path absolute = filename.Absolute( parent );     // bar.txt          =>   CurrentPath()/bar.txt
    const tools::Path relative1 = base.Relative();          // C:/foo/bar.txt   =>   foo/bar.txt
    const tools::Path relative2 = base.Relative( parent );  // C:/foo/bar.txt with p=C:/foo  =>  bar.txt

    absolute.Normalize();                                   // on windows, concatenation happens with \\ instead of /, so use Normalize to replace \\ by /

    BOOST_CHECK_EQUAL( root,        "C:/" );
    BOOST_CHECK_EQUAL( parent,      "C:/foo" );
    BOOST_CHECK_EQUAL( filename,    "bar.txt" );
    BOOST_CHECK_EQUAL( basename,    "bar" );
    BOOST_CHECK_EQUAL( extension,   ".txt" );
    BOOST_CHECK_EQUAL( absolute,    "C:/foo/bar.txt" );
    BOOST_CHECK_EQUAL( relative1,   "foo/bar.txt" );
    BOOST_CHECK_EQUAL( relative2,   "bar.txt" );
}

// -----------------------------------------------------------------------------
// Operators
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_operators )
{
    const tools::Path path = "C:/foo";
    const tools::Path basename = "bar";
    const tools::Path extension = ".txt";
    const tools::Path filename = "bar.txt";
    const tools::Path fullpath = "C:/foo/bar.txt";

    tools::Path tmp( "C:/foo" );
    BOOST_CHECK_EQUAL( tmp == path, true );
    BOOST_CHECK_EQUAL( tmp != fullpath, true );
    BOOST_CHECK_EQUAL( tmp != path, false );
    BOOST_CHECK_EQUAL( tmp == fullpath, false );

    tmp = path;
    BOOST_CHECK_EQUAL( tmp, "C:/foo" );

    tmp = path / filename;
    tmp.Normalize();
    BOOST_CHECK_EQUAL( tmp, fullpath );

    tmp = path;
    tmp /= filename;
    tmp.Normalize();
    BOOST_CHECK_EQUAL( tmp, fullpath );

    tmp = basename + extension;
    BOOST_CHECK_EQUAL( tmp, filename );

    tmp = basename;
    tmp += extension;
    BOOST_CHECK_EQUAL( tmp, filename );

    BOOST_CHECK_EQUAL( path < fullpath, true );
    BOOST_CHECK_EQUAL( filename < fullpath, false );
    BOOST_CHECK_EQUAL( filename < extension, false );
}

// -----------------------------------------------------------------------------
// Queries
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_queries )
{
    tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    tools::Path tmpDir = tools::Path::FromUnicode( dir.path().wstring() );
    tools::Path emptyPath = "";
    tools::Path absolutePath = "C:/foobar.txt";
    tools::Path relativePath = "./foobar.txt";
    tools::Path fileWithoutExtension = tmpDir / "foobar";
    tools::Path regularFile = tmpDir / "foobar.txt";

    BOOST_CHECK_EQUAL( tmpDir.Exists(), true );
    BOOST_CHECK_EQUAL( tmpDir.IsDirectory(), true );

    BOOST_CHECK_EQUAL( tmpDir.IsEmpty(), false );
    BOOST_CHECK_EQUAL( emptyPath.IsEmpty(), true );

    BOOST_CHECK_EQUAL( regularFile.Exists(), false );
    BOOST_CHECK_EQUAL( regularFile.IsRegularFile(), false );
    CreateFile( regularFile );
    BOOST_CHECK_EQUAL( regularFile.Exists(), true );
    BOOST_CHECK_EQUAL( regularFile.IsRegularFile(), true );

    BOOST_CHECK_EQUAL( relativePath.IsAbsolute(), false );
    BOOST_CHECK_EQUAL( relativePath.IsRelative(), true );
    BOOST_CHECK_EQUAL( relativePath.HasRootDirectory(), false );
    BOOST_CHECK_EQUAL( absolutePath.IsAbsolute(), true );
    BOOST_CHECK_EQUAL( absolutePath.IsRelative(), false );
    BOOST_CHECK_EQUAL( absolutePath.HasRootDirectory(), true );

    BOOST_CHECK_EQUAL( fileWithoutExtension.HasExtension(), false );
    BOOST_CHECK_EQUAL( regularFile.HasExtension(), true );

    BOOST_CHECK_EQUAL( emptyPath.HasFilename(), false );
    BOOST_CHECK_EQUAL( regularFile.HasFilename(), true );
}

// -----------------------------------------------------------------------------
// Modifiers
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_modifiers )
{
    tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    const tools::Path path = "C:/foo/bar.txt";
    tools::Path tmp = path;
    BOOST_CHECK_EQUAL( tmp.IsEmpty(), false );
    tmp.Clear();
    BOOST_CHECK_EQUAL( tmp.IsEmpty(), true );

    tmp = path;
    tmp.MakePreferred();
    BOOST_CHECK_EQUAL( tmp, "C:\\foo\\bar.txt" );
    tmp.Normalize();
    BOOST_CHECK_EQUAL( tmp, path );

    BOOST_CHECK_EQUAL( tmp.Extension(), ".txt" );
    tmp.ReplaceExtension( ".foobar" );
    BOOST_CHECK_EQUAL( tmp.Extension(), ".foobar" );
    BOOST_CHECK_EQUAL( tmp, "C:/foo/bar.foobar" );

    tmp = "C:/foo/bar/../bar.txt";
    tmp.SystemComplete();
    BOOST_CHECK_EQUAL( tmp, path );
}

// -----------------------------------------------------------------------------
// List operations
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_list_operations )
{
    tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    const tools::Path tmpDir = tools::Path::FromUnicode( dir.path().wstring() );
    PopulateSampleDirectory( tmpDir );
    PopulateSampleDirectory( tmpDir, "foobar" );
    ( tmpDir / "dir1" ).CreateDirectories();
    ( tmpDir / "dir2" ).CreateDirectories();

    BOOST_CHECK_EQUAL( tmpDir.ListFiles().size(), 20 );
    BOOST_CHECK_EQUAL( tmpDir.ListDirectories().size(), 2 );
    BOOST_CHECK_EQUAL( tmpDir.ListElements().size(), 22 );
    BOOST_CHECK_EQUAL( tmpDir.ListElements( boost::bind( &CheckFileName, L"foobar", _1 ) ).size(), 10 );
}

// -----------------------------------------------------------------------------
// File operations
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_file_operations )
{
    tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    const tools::Path tmpDir = tools::Path::FromUnicode( dir.path().wstring() );
    const tools::Path file1 = tmpDir / "test1.txt";
    const tools::Path file2 = tmpDir / "test2.txt";
    const tools::Path dir1 = tmpDir / "test1";
    const tools::Path dir2 = tmpDir / "test2";

    BOOST_CHECK_EQUAL( file1.Exists(), false );
    BOOST_CHECK_EQUAL( file2.Exists(), false );
    CreateFile( file1 );
    BOOST_CHECK_EQUAL( file1.Exists(), true );
    file1.Rename( file2 );
    BOOST_CHECK_EQUAL( file1.Exists(), false );
    BOOST_CHECK_EQUAL( file2.Exists(), true );
    file2.Remove();
    BOOST_CHECK_EQUAL( file2.Exists(), false );
    CreateFile( file1 );
    file1.Copy( file2 );
    BOOST_CHECK_EQUAL( file1.Exists(), true );
    BOOST_CHECK_EQUAL( file2.Exists(), true );
    file1.Remove();
    file2.Remove();
    BOOST_CHECK_EQUAL( file1.Exists(), false );
    BOOST_CHECK_EQUAL( file2.Exists(), false );

    BOOST_CHECK_EQUAL( dir1.Exists(), false );
    dir1.CreateDirectories();
    BOOST_CHECK_EQUAL( dir1.Exists(), true );
    BOOST_CHECK_EQUAL( dir1.IsDirectory(), true );
    PopulateSampleDirectory( dir1 );
    PopulateSampleDirectory( dir1, "foobar" );
    BOOST_CHECK_EQUAL( dir1.ListFiles().size(), 20 );
    BOOST_CHECK_EQUAL( dir2.Exists(), false );
    dir1.Copy( dir2 );
    BOOST_CHECK_EQUAL( dir2.Exists(), true );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 20 );
    dir1.Copy( dir2, tools::Path::IgnoreIfExists );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 20 );
    dir1.Copy( dir2, tools::Path::OverwriteIfExists );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 20 );
    BOOST_CHECK_THROW( dir1.Copy( dir2, tools::Path::FailIfExists ), std::exception );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 20 );
    dir2.RemoveAll();
    BOOST_CHECK_EQUAL( dir2.Exists(), false );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 0 );

    dir1.Copy( boost::bind( &CheckFileName, L"foobar", _1 ), dir2 );
    BOOST_CHECK_EQUAL( dir2.ListFiles().size(), 10 );
}

// -----------------------------------------------------------------------------
// Iterator
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_iterator )
{
    tools::TemporaryDirectory dir( "pathtest-", temp_directory );
    const tools::Path tmpDir = tools::Path::FromUnicode( dir.path().wstring() );
    PopulateSampleDirectory( tmpDir, "test" );

    for( auto it = tmpDir.begin(); it != tmpDir.end(); ++it )
    {
        const tools::Path& path = *it;
        BOOST_CHECK_EQUAL( path.Exists(), true );
        BOOST_CHECK_EQUAL( path.IsRegularFile(), true );

        const std::wstring baseName = path.BaseName().ToUnicode();
        BOOST_CHECK_EQUAL( baseName.find( L"test" ) != std::wstring::npos, true );
    }
}

// -----------------------------------------------------------------------------
// Static path
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_static_path )
{
    BOOST_CHECK_EQUAL( GetCurrentPath().c_str(), tools::Path::CurrentPath().ToUnicode().c_str() );
    BOOST_CHECK_EQUAL( GetTempPath().c_str(), tools::Path::TemporaryPath().ToUnicode().c_str() );
}

// -----------------------------------------------------------------------------
// Serialization
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_serialization )
{
    //tools::TemporaryDirectory dir( "pathtest-", temp_directory );
    //tools::Path archiveFile = tools::Path::FromUnicode( dir.path().wstring() ) / "archive";
    //tools::Path savedPath = "C:/foo/bar.txt";
    //tools::Path loadedPath;
    //{
    //    tools::Ofstream file( archiveFile, std::ios::out | std::ios::binary );
    //    if( !file || !file.is_open() )
    //        throw MASA_EXCEPTION( "Cannot open file '" + strFileName.ToUTF8() + "'" );
    //    MIL_CheckPointOutArchive* pArchive = new MIL_CheckPointOutArchive( file );
    //    assert( pArchive != 0 );
    //    savedPath.save( *pArchive );
    //    file.close();
    //}

    //{
    //    DummyObjectResolver resolver;
    //    tools::Ifstream file( archiveFile, std::ios::in | std::ios::binary );
    //    if( !file || !file.is_open() )
    //        throw MASA_EXCEPTION( "Cannot open file '" + config.BuildCheckpointChildFile( "data" ).ToUTF8() + "'" );
    //    MIL_CheckPointInArchive* pArchive = new MIL_CheckPointInArchive( file, resolver );
    //    assert( pArchive != 0 );
    //    loadedPath.load( *pArchive );
    //    file.close();
    //}

    //BOOST_CHECK_EQUAL( savedPath, loadedPath );
    BOOST_TODO;
}

// -----------------------------------------------------------------------------
// Stream
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_stream )
{
    //tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    //std::ostream& operator<<( std::ostream& os, const Path& path );
    //xml::xostream& operator<<( xml::xostream& xos, const tools::Path& path );
    //xml::xistream& operator>>( xml::xistream& xis, tools::Path& path );

    BOOST_TODO;
}

// -----------------------------------------------------------------------------
// Option
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_option )
{
    //tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    //void validate( boost::any& v, const std::vector< std::string >& values, tools::Path*, int );

    BOOST_TODO;
}

// -----------------------------------------------------------------------------
// Conversion
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( test_path_conversion )
{
    //tools::TemporaryDirectory dir( "pathtest-", temp_directory );

    //const std::string ToLocal() const;
    //const std::string ToUTF8() const;
    //const std::wstring ToUnicode() const;
    //static Path FromLocal( const std::string& path );
    //static Path FromUTF8( const std::string& path );
    //static Path FromUnicode( const std::wstring& path );

    BOOST_TODO;
}
