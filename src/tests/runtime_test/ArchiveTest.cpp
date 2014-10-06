// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime_test_pch.h"

#include <runtime/FileSystem.h>
#include <runtime/Io.h>
#include <runtime/Scoper.h>
#include <tools/BoostTest.h>

#include <cpplog/cpplog.hpp>
#include <boost/filesystem.hpp>
#include <set>

using namespace runtime;

namespace
{
    MOCK_BASE_CLASS( MockLog, cpplog::BaseLogger )
    {
        MOCK_METHOD( sendLogMessage, 1 );
        MockLog()
        {
            MOCK_EXPECT( sendLogMessage ).returns( true );
        }
    };

    struct VirtualDevice : public io::Writer_ABC
                         , public io::Reader_ABC
    {
        VirtualDevice()
        {
            // NOTHING
        }

        int Write( const void* data, size_t size )
        {
            const size_t previous = buffer_.size();
            buffer_.resize( previous + size, 0 );
            memcpy( &buffer_[previous], data, size );
            return static_cast< int >( size );
        }

        int Read( void* data, size_t size )
        {
            const size_t next = std::min( size, buffer_.size() );
            if( next )
                memcpy( data, &buffer_[0], next );
            buffer_.erase( buffer_.begin(), buffer_.begin() + next );
            return static_cast< int >( next );
        }

        std::vector< char > buffer_;
    };

    Path GetSubPath( const Path& root, const Path& path )
    {
        Path reply;
        auto a = root.begin();
        for( auto b = path.begin(); b != path.end(); ++b )
            if( a != root.end() )
                ++a;
            else
                reply /= *b;
        return reply;
    }

    struct Snapshot
    {
        std::set< Path > paths;
        std::string crc;
        size_t size;
    };

    void Compare( const Snapshot& a, const Snapshot& b )
    {
        BOOST_CHECK_EQUAL( a.size, b.size );
        BOOST_CHECK_EQUAL( a.crc, b.crc );
        BOOST_CHECK_EQUAL_COLLECTIONS( a.paths.begin(), a.paths.end(), b.paths.begin(), b.paths.end() );
    }

    Snapshot MakeSnapshot( const FileSystem_ABC& fs, const Path& src )
    {
        Snapshot dst;
        dst.size = 0;
        dst.crc = fs.Checksum( src, [&]( const Path& path ) -> bool {
            dst.paths.insert( GetSubPath( src, path ) );
            return true;
        }, dst.size );
        return dst;
    }

    void TestArchiveCycle( const std::string& suffix, ArchiveFormat fmt )
    {
        MockLog log;
        FileSystem fs( log );
        VirtualDevice dev;
        const auto input = testOptions.GetDataPath( "archive" ).ToBoost();
        const auto output = testOptions.GetTempDir().ToBoost() / suffix;
        const Scoper removeOutput = [&](){ fs.Remove( output ); };
        const auto ref = MakeSnapshot( fs, input );
        {
            auto packer = fs.Pack( dev, fmt );
            packer->Pack( input, []( const Path& path ) {
                return path != "echowin32.exe";
            } );
            VirtualDevice file;
            fs.LimitedReadFile( file, input / "echowin32.exe", 0 );
            packer->PackEntry( "echowin32.exe", &file.buffer_[0], file.buffer_.size() );
        }
        {
            auto unpacker = fs.Unpack( output, dev, 0 );
            unpacker->Unpack();
        }
        const auto got = MakeSnapshot( fs, output );
        Compare( ref, got );
    }
}

BOOST_AUTO_TEST_CASE( archive_cycle )
{
    TestArchiveCycle( "zip",    ARCHIVE_FMT_ZIP );
    TestArchiveCycle( "tar.gz", ARCHIVE_FMT_TAR_GZ );
}

namespace
{
    void CheckUnpack( const FileSystem_ABC& fs, const std::string& name )
    {
        const auto path = testOptions.GetDataPath( tools::Path::FromUTF8( name ) ).ToBoost();
        const auto output = fs.MakeAnyPath( testOptions.GetTempDir().ToBoost() );
        const Scoper removeOutput = [&](){ fs.Remove( output ); };
        const auto data = fs.ReadFile( path );
        VirtualDevice input;
        input.Write( data.data(), data.size() );
        {
            auto unpacker = fs.Unpack( output, input, 0 );
            unpacker->Unpack();
        }
    }
}

BOOST_AUTO_TEST_CASE( resilient_against_7z_archives )
{
    MockLog log;
    FileSystem fs( log );
    CheckUnpack( fs, "crash.zip" ); // 7z + pure unicode filename
    CheckUnpack( fs, "crash.tar.gz" ); // 7z + pure unicode filename
    // I don't know how to handle this case properly, but we must not crash
    BOOST_CHECK_THROW( CheckUnpack( fs, "crash2.zip" ), std::runtime_error ); // 7z + locale filename
}

BOOST_AUTO_TEST_CASE( fs_copy_directory_is_unicode )
{
    MockLog log;
    FileSystem fs( log );
    const auto input = testOptions.GetDataPath( "archive" ).ToBoost();
    const auto output = fs.MakeAnyPath( testOptions.GetTempDir().ToBoost() );
    fs.CopyDirectory( input, output );
    Compare( MakeSnapshot( fs, input ), MakeSnapshot( fs, output ) );
    fs.Remove( output );
}
