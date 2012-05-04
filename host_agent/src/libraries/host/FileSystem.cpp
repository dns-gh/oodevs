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
typedef struct archive Archive;
typedef struct archive_entry ArchiveEntry;

void AbortArchive( cpplog::BaseLogger& log, Archive* arc )
{
    LOG_ERROR( log ) << "[archive] " << archive_error_string( arc );
    throw std::runtime_error( "[archive] Operation aborted" );
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
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Unpack
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void FileSystem::Unpack( const boost::filesystem::path& input, const boost::filesystem::path& prefix ) const
{
    boost::shared_ptr< Archive > src( archive_read_new(), archive_read_free );
    archive_read_support_format_zip( src.get() );
    int err = archive_read_open_filename_w( src.get(), input.wstring().c_str(), 1<<16 );
    if( err != ARCHIVE_OK )
        AbortArchive( log_, src.get() );

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

        const boost::filesystem::path next = prefix / archive_entry_pathname_w( entry );
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