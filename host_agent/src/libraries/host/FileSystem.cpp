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
#include "runtime/Utf8.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/make_shared.hpp>

#define  LIBARCHIVE_STATIC
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>
#undef   CopyFile

using namespace host;

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
bool FileSystem::IsFile( const boost::filesystem::path& path ) const
{
    return boost::filesystem::is_regular_file( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsDirectory( const boost::filesystem::path& path ) const
{
    return boost::filesystem::is_directory( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Exists
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::Exists( const boost::filesystem::path& path ) const
{
    return boost::filesystem::exists( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::CopyDirectory( const boost::filesystem::path& src, const boost::filesystem::path& dst ) const
{
    // TODO Use wrecursive_directory_iterator
    if( IsFile( src ) )
        return boost::filesystem::copy_file( src, dst / src.filename() );
    else if( !IsDirectory( src ) )
        return;
    const boost::filesystem::path sub = dst / src.filename();
    if( !boost::filesystem::create_directory( sub ) )
        throw std::runtime_error( "unable to create " + sub.string() );
    for( boost::filesystem::directory_iterator it( src ); it != boost::filesystem::directory_iterator(); ++it )
        CopyDirectory( *it, sub );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyFile
// Created: BAX 2012-03-29
// -----------------------------------------------------------------------------
void FileSystem::CopyFile( const boost::filesystem::path& src, const boost::filesystem::path& dst ) const
{
    boost::filesystem::copy_file( src, dst / src.filename() );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakeDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::MakeDirectory( const boost::filesystem::path& path ) const
{
    boost::filesystem::create_directories( path );
}

namespace
{
    std::string TryRemove( const boost::filesystem::path& path )
    {
        try
        {
            boost::filesystem::remove_all( path );
            return std::string();
        }
        catch( std::exception& err )
        {
            return err.what();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Remove
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::Remove( const boost::filesystem::path& path ) const
{
    std::string reply = TryRemove( path );
    if( !reply.empty() )
        reply = TryRemove( path );
    if( !reply.empty() )
        LOG_ERROR( log_ ) << "[file] Unable to remove path " + path.string() + ", " + reply;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::WriteFile
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void FileSystem::WriteFile( const boost::filesystem::path& path, const std::string& content ) const
{
    boost::filesystem::ofstream( path ) << content;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::ReadFile
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string FileSystem::ReadFile( const boost::filesystem::path& path ) const
{
    boost::filesystem::ifstream ifs( path );
    return std::string( std::istreambuf_iterator< char >( ifs ), std::istreambuf_iterator< char >() );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Glob
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::vector< boost::filesystem::path > FileSystem::Glob( const boost::filesystem::path& path, const std::wstring& name ) const
{
    std::vector< boost::filesystem::path > paths;
    if( IsDirectory( path ) )
        for( boost::filesystem::recursive_directory_iterator it( path ); it != boost::filesystem::recursive_directory_iterator(); ++it )
            if( IsFile( *it ) && it->path().filename() == name )
                paths.push_back( *it );
    return paths;
}

namespace
{
template< typename T >
std::vector< boost::filesystem::path > Iterate( const boost::filesystem::path& path )
{
    std::vector< boost::filesystem::path > paths;
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
std::vector< boost::filesystem::path > FileSystem::Walk( const boost::filesystem::path& path, bool recurse ) const
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
    throw std::runtime_error( "[archive] Operation aborted, " + err );
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
    Unpacker( cpplog::BaseLogger& log, const boost::filesystem::path& output, std::istream& stream )
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

    static __LA_SSIZE_T Read( Archive* arc, void* userdata, const void** buffer )
    {
        Unpacker* it = static_cast< Unpacker* >( userdata );
        __LA_SSIZE_T fill = 0;
        *buffer = &it->buffer_[0];
        while( fill < 1 && !it->stream_.eof() )
        {
            it->stream_.read( &it->buffer_[ fill ], it->buffer_.size() - fill );
            fill += it->stream_.gcount();
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

            const boost::filesystem::path next = output_ / archive_entry_pathname_w( entry );
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
    const boost::filesystem::path output_;
    std::istream& stream_;
    std::vector< char > buffer_;
};
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Unpack
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
FileSystem_ABC::T_Unpacker FileSystem::Unpack( const boost::filesystem::path& output, std::istream& src ) const
{
    return boost::make_shared< Unpacker >( log_, output, src );
}