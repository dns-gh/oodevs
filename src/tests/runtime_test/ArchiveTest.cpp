// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime_test.h"

#include <runtime/FileSystem.h>
#include <runtime/Io.h>
#include <tools/BoostTest.h>

#include <cpplog/cpplog.hpp>
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

    void TestArchiveCycle( const std::string& suffix, ArchiveFormat fmt )
    {
        MockLog log;
        FileSystem fs( log );
        VirtualDevice dev;
        const auto input = testOptions.GetDataPath( "archive" ).ToBoost();
        const auto output = testOptions.GetTempDir().ToBoost() / suffix;
        size_t refsize = 0;
        std::set< Path > refpaths;
        const std::string refcrc = fs.Checksum( input, [&]( const Path& path ) -> bool {
            refpaths.insert( GetSubPath( input, path ) );
            return true;
        }, refsize );
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
        size_t read = 0;
        std::set< Path > outpaths;
        const std::string crc = fs.Checksum( output, [&]( const Path& path ) -> bool {
            outpaths.insert( GetSubPath( output, path ) );
            return true;
        }, read );
        BOOST_CHECK_EQUAL( read, refsize );
        BOOST_CHECK_EQUAL( crc, refcrc );
        BOOST_CHECK_EQUAL_COLLECTIONS( refpaths.begin(), refpaths.end(), outpaths.begin(), outpaths.end() );
        fs.Remove( output );
    }
}

BOOST_AUTO_TEST_CASE( archive_cycle )
{
    TestArchiveCycle( "zip",    ARCHIVE_FMT_ZIP );
    TestArchiveCycle( "tar.gz", ARCHIVE_FMT_TAR_GZ );
}