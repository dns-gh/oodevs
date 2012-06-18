// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FileSystem.h"

#include "cpplog/cpplog.hpp"
#include "Utf8.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#define  LIBARCHIVE_STATIC
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>
#undef   CopyFile

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 4245 )
#endif
#include <boost/crc.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace runtime;
using runtime::Utf8Convert;

// -----------------------------------------------------------------------------
// Name: FileSystem::FileSystem
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
FileSystem::FileSystem( cpplog::BaseLogger& log )
    : log_( log )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileSystem::~FileSystem
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
FileSystem::~FileSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsFile
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsFile( const Path& path ) const
{
    return boost::filesystem::is_regular_file( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsDirectory( const Path& path ) const
{
    return boost::filesystem::is_directory( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Exists
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::Exists( const Path& path ) const
{
    return boost::filesystem::exists( path );
}

namespace
{
Path MovePath( const Path& dst, const std::string& prefix, const Path& src )
{
    return dst / Utf8Convert( src ).substr( prefix.size() );
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::CopyDirectory( const Path& src, const Path& dst ) const
{
    const std::string prefix = Utf8Convert( src );
    for( boost::filesystem::recursive_directory_iterator it( src ); it != boost::filesystem::recursive_directory_iterator(); ++it )
        if( boost::filesystem::is_directory( it->status() ) )
            boost::filesystem::create_directory( MovePath( dst, prefix, *it ) );
        else if( boost::filesystem::is_regular_file( it->status() ) )
            boost::filesystem::copy_file( *it, MovePath( dst, prefix, *it ) );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyFile
// Created: BAX 2012-03-29
// -----------------------------------------------------------------------------
bool FileSystem::CopyFile( const Path& src, const Path& dst ) const
{
    try
    {
        boost::filesystem::copy_file( src, dst / src.filename() );
        return true;
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to copy " << src.string() << " to " << dst.string();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakePaths
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::MakePaths( const Path& path ) const
{
    boost::filesystem::create_directories( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakePath
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::MakePath( const Path& path ) const
{
    return boost::filesystem::create_directory( path );
}

namespace
{
bool TryRemove( const Path& path, cpplog::BaseLogger* log )
{
    try
    {
        boost::filesystem::remove_all( path );
        return true;
    }
    catch( const std::exception& err )
    {
        if( log )
        {
            LOG_ERROR( *log ) << "[file] " << err.what();
            LOG_ERROR( *log ) << "[file] Unable to remove path " << path.string();
        }
    }
    return false;
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Remove
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::Remove( const Path& path ) const
{
    bool done = TryRemove( path, 0 );
    if( !done )
        done = TryRemove( path, &log_ );
    return done;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Rename
// Created: BAX 2012-05-30
// -----------------------------------------------------------------------------
bool FileSystem::Rename( const Path& src, const Path& dst ) const
{
    try
    {
        boost::filesystem::rename( src, dst );
        return true;
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to rename " << src.string() << " to " << dst.string();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::WriteFile
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
bool FileSystem::WriteFile( const Path& path, const std::string& content ) const
{
    try
    {
        boost::filesystem::ofstream ofs( path );
        ofs << content;
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to write file " << path.string();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::ReadFile
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string FileSystem::ReadFile( const Path& path ) const
{
    try
    {
        boost::filesystem::ifstream ifs( path );
        if( !ifs.fail() )
            return std::string( std::istreambuf_iterator< char >( ifs ), std::istreambuf_iterator< char >() );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to read file " << path.string();
    }
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Glob
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::vector< Path > FileSystem::Glob( const Path& path, const Path& name ) const
{
    std::vector< Path > paths;
    if( IsDirectory( path ) )
        for( boost::filesystem::recursive_directory_iterator it( path ); it != boost::filesystem::recursive_directory_iterator(); ++it )
            if( IsFile( *it ) && it->path().filename() == name )
                paths.push_back( *it );
    return paths;
}

namespace
{
template< typename T >
std::vector< Path > Iterate( const Path& path )
{
    std::vector< Path > paths;
    if( boost::filesystem::is_directory( path ) )
        for( T it( path); it != T(); ++it )
            paths.push_back( *it );
    return paths;
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Walk
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
std::vector< Path > FileSystem::Walk( const Path& path, bool recurse ) const
{
    if( recurse )
        return Iterate< boost::filesystem::recursive_directory_iterator >( path );
    else
        return Iterate< boost::filesystem::directory_iterator >( path );
}

namespace
{
typedef struct archive Archive;
typedef struct archive_entry ArchiveEntry;

void AbortArchive( cpplog::BaseLogger& log, Archive* arc )
{
    const std::string err = archive_error_string( arc );
    LOG_ERROR( log ) << "[archive] " << err;
    throw std::runtime_error( "Operation aborted" );
}

void CheckArchiveCode( cpplog::BaseLogger& log, Archive* arc, int err )
{
    if( err < ARCHIVE_WARN )
        AbortArchive( log, arc );
    else
        LOG_WARN( log ) << "[archive] " << archive_error_string( arc );
}

void CopyBlocks( cpplog::BaseLogger& log, Archive* src, Archive* dst )
{
    for( ;; )
    {
        const void* buffer; size_t size; int64_t offset;
        int err = archive_read_data_block( src, &buffer, &size, &offset );
        if( err == ARCHIVE_EOF )
            return;
        if( err != ARCHIVE_OK )
            AbortArchive( log, src );
        __LA_SSIZE_T len = archive_write_data_block( dst, buffer, size, offset );
        if( len != ARCHIVE_OK )
            AbortArchive( log, dst );
    }
}

struct Unpacker : public Unpacker_ABC
{
    Unpacker( cpplog::BaseLogger& log, const Path& output, std::istream& stream )
        : log_   ( log )
        , output_( output )
        , stream_( stream )
        , buffer_( UINT16_MAX )
    {
        // NOTHING
    }

    ~Unpacker()
    {
        // NOTHING
    }

    static __LA_SSIZE_T Read( Archive* /*arc*/, void* userdata, const void** buffer )
    {
        Unpacker* it = static_cast< Unpacker* >( userdata );
        __LA_SSIZE_T fill = 0;
        *buffer = &it->buffer_[0];
        while( fill < 1 && !it->stream_.eof() )
        {
            it->stream_.read( &it->buffer_[ fill ], it->buffer_.size() - fill );
            fill += static_cast< __LA_SSIZE_T >( it->stream_.gcount() );
        }
        return fill;
    }

    void Unpack()
    {
        boost::shared_ptr< Archive > src( archive_read_new(), archive_read_free );
        archive_read_support_format_zip( src.get() );
        archive_read_set_read_callback( src.get(), &Unpacker::Read );
        archive_read_set_callback_data( src.get(), this );
        int err = archive_read_open1( src.get() );
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( src.get() ) );

        boost::shared_ptr< Archive > dst( archive_write_disk_new(), archive_write_free );
        int flags = ARCHIVE_EXTRACT_TIME
              | ARCHIVE_EXTRACT_PERM
              | ARCHIVE_EXTRACT_ACL
              | ARCHIVE_EXTRACT_FFLAGS;
        archive_write_disk_set_options( dst.get(), flags );
        archive_write_disk_set_standard_lookup( dst.get() );

        for( ;; )
        {
            ArchiveEntry* entry;
            err = archive_read_next_header( src.get(), &entry );
            if( err == ARCHIVE_EOF )
                break;
            if( err != ARCHIVE_OK )
                CheckArchiveCode( log_, src.get(), err );

            const Path next = output_ / archive_entry_pathname_w( entry );
            archive_entry_update_pathname_utf8( entry, runtime::Utf8Convert( next ).c_str() );

            err = archive_write_header( dst.get(), entry );
            if( err != ARCHIVE_OK )
                CheckArchiveCode( log_, dst.get(), err );
            else if( archive_entry_size( entry ) > 0 )
                CopyBlocks( log_, src.get(), dst.get() );
            err = archive_write_finish_entry( dst.get() );
            if( err != ARCHIVE_OK )
                CheckArchiveCode( log_, dst.get(), err );
        }
        archive_read_close( src.get() );
        archive_write_close( dst.get() );
    }

    cpplog::BaseLogger& log_;
    const Path output_;
    std::istream& stream_;
    std::vector< char > buffer_;
};
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Unpack
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
FileSystem_ABC::T_Unpacker FileSystem::Unpack( const Path& output, std::istream& src ) const
{
    return boost::make_shared< Unpacker >( boost::ref( log_ ), output, boost::ref( src ) );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Checksum
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
std::string FileSystem::Checksum( const Path& root, const FileSystem_ABC::T_Predicate& predicate ) const
{
    boost::crc_32_type sum;
    std::vector< char > buffer( UINT16_MAX );
    for( boost::filesystem::recursive_directory_iterator it( root ); it != boost::filesystem::recursive_directory_iterator(); ++it )
    {
        if( !boost::filesystem::is_regular_file( it.status() ) )
            continue;
        if( predicate && !predicate( it->path() ) )
            continue;
#ifdef _MSC_VER
        // msvc does not support utf-8 strings
        std::ifstream in( it->path().wstring(), std::ifstream::binary );
#else
        std::ifstream in( Utf8Convert( *it ).c_str(), std::ifstream::binary );
#endif
        if( in.bad() )
        {
            LOG_ERROR( log_ ) << "[file] Unable to checksum " << it->path().string();
            return std::string();
        }
        while( in.good() )
        {
            in.read( &buffer[0], buffer.size() );
            sum.process_bytes( &buffer[0], static_cast< size_t >( in.gcount() ) );
        }
    }
    const size_t size = sprintf( &buffer[0], "%08X", sum.checksum() );
    return std::string( &buffer[0], size );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakeAnyPath
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
Path FileSystem::MakeAnyPath( const Path& root ) const
{
    for( uint16_t idx = 0; ; ++idx )
    {
        const Path next = root / boost::lexical_cast< std::string >( idx );
        if( boost::filesystem::exists( next ) )
            continue;
        else if( MakePath( next ) )
            return next;
    }
}

// -----------------------------------------------------------------------------
// Name: FileSystem::GetLastWrite
// Created: BAX 2012-06-08
// -----------------------------------------------------------------------------
std::time_t FileSystem::GetLastWrite( const Path& file ) const
{
    return boost::filesystem::last_write_time( file );
}
