// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#define  __STDC_LIMIT_MACROS
#define  NOMINMAX
#include "FileSystem.h"

#include "cpplog/cpplog.hpp"
#include "Io.h"
#include "Scoper.h"
#include "Utf8.h"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include <stdint.h>
#include <fcntl.h>
#include <iostream>
#include <zlib/zlib.h>

#ifdef WIN32
#include <io.h>
#else
#define O_BINARY 0
#endif

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
using runtime::Utf8;

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
Path MovePath( const Path& dst, const std::wstring& prefix, const Path& src )
{
    return dst / src.wstring().substr( prefix.size() );
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::CopyDirectory( const Path& src, const Path& dst ) const
{
    const std::wstring prefix = src.wstring();
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

namespace
{
void RemoveFile( const FileSystem_ABC& fs, const Path& tmp )
{
    if( !tmp.empty() )
        fs.Remove( tmp );
}

int OpenDescriptor( const Path& path )
{
#ifdef _MSC_VER
    int fd;
    _wsopen_s( &fd, path.wstring().c_str(), O_WRONLY | O_CREAT | O_EXCL | O_BINARY, _SH_DENYRW, _S_IREAD | _S_IWRITE );
    return fd;
#else
    return open( Utf8( path ).c_str(), O_WRONLY | O_CREAT | O_EXCL | O_BINARY );
#endif
}

#ifdef _MSC_VER
#define write _write
#define close _close
#endif

size_t WriteToFile( int fd, const char* data, size_t size )
{
    size_t fill = 0;
    while( fill < size )
    {
        int next = write( fd, data + fill, static_cast< unsigned int >( size - fill ) );
        if( next == -1 )
            return fill;
        fill += next;
    }
    return size;
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::WriteFile
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
bool FileSystem::WriteFile( const Path& path, const std::string& content ) const
{
    try
    {
        int fd = -1;
        Path tmp;
        for( size_t retry = 0; retry < 3 && fd == -1; ++retry )
        {
            boost::system::error_code ec;
            tmp = boost::filesystem::unique_path( Path( path ).remove_filename() / "%%%%%%%%.%%%", ec );
            if( ec )
                continue;
            fd = OpenDescriptor( tmp );
        }
        if( fd == -1 )
            throw std::runtime_error( "Unable to create temporary file" );
        Scoper removeTmp( boost::bind( &RemoveFile, boost::cref( *this ), boost::cref( tmp ) ) );
        const size_t fill = WriteToFile( fd, content.c_str(), content.size() );
        close( fd );
        if( fill != content.size() )
            throw std::runtime_error( "Unable to write temporary file contents" );
        if( !Rename( tmp, path ) )
            throw std::runtime_error( "Unable to rename " + tmp.string() + " to " + path.string() );
        tmp.clear();
        return true;
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to write file " << path.string();
    }
    return false;
}

#ifdef _MSC_VER
#undef write
#undef close
#endif

// -----------------------------------------------------------------------------
// Name: FileSystem::ReadFile
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string FileSystem::ReadFile( const Path& path ) const
{
    try
    {
        boost::filesystem::ifstream ifs( path, std::ifstream::binary );
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
// Name: FileSystem::ReadFileWithLimitSize
// Created: NPT 2013-07-25
// -----------------------------------------------------------------------------
void FileSystem::LimitedReadFile( io::Writer_ABC& sink, const Path& path, int limit ) const
{
    try
    {
        char buffer[ 4 * 1024 ];
        boost::filesystem::ifstream ifs( path, std::ifstream::binary );
        if( ifs.fail() )
            return;
        if( limit )
        {
            ifs.seekg ( 0, ifs.end );
            int fileSize = (int)ifs.tellg();
            ifs.seekg ( 0, ifs.beg );
            int pos = std::max( fileSize - limit, 0 );
            if( pos > 0 )
            {
                ifs.seekg( pos );
                while( ifs.get() != '\n' )
                    continue;
            }
        }
        while( !ifs.eof() )
        {
            ifs.read( buffer, sizeof( buffer ) );
            const size_t len = static_cast< size_t >( ifs.gcount() );
            sink.Write( buffer, len );
        }
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to read file " << path.string();
    }
}

namespace
{
template< typename T, typename U >
void Iterate( const Path& path, const U& predicate )
{
    if( boost::filesystem::is_directory( path ) )
        for( T it( path), end; it != end; ++it )
            if( !predicate( it->path() ) )
                return;
}
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Walk
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void FileSystem::Walk( const Path& path, bool recurse, const T_Predicate& predicate ) const
{
    if( recurse )
        Iterate< boost::filesystem::recursive_directory_iterator >( path, predicate );
    else
        Iterate< boost::filesystem::directory_iterator >( path, predicate );
}

namespace
{
typedef struct archive Archive;
typedef struct archive_entry ArchiveEntry;

void AbortArchive( cpplog::BaseLogger& log, Archive* arc )
{
    const char* err = archive_error_string( arc );
    if( err )
        LOG_ERROR( log ) << "[archive] " << std::string( err );
    throw std::runtime_error( "Operation aborted" );
}

void CheckArchiveCode( cpplog::BaseLogger& log, Archive* arc, int err )
{
    if( err < ARCHIVE_WARN )
        AbortArchive( log, arc );
    else
        LOG_WARN( log ) << "[archive] " << archive_error_string( arc );
}

const wchar_t* StripPathPrefix( const wchar_t* input, size_t diff )
{
    input += diff;
    return *input == L'/' ? ++input : input;
}

void CopyBlocks( cpplog::BaseLogger& log, Archive* src, Archive* dst, io::Writer_ABC* writer )
{
    for( ;; )
    {
        const void* buffer; size_t size; int64_t offset;
        int err = archive_read_data_block( src, &buffer, &size, &offset );
        if( err == ARCHIVE_EOF )
            return;
        if( err != ARCHIVE_OK )
            AbortArchive( log, src );
        if( !size )
            continue;
        if( writer )
            writer->Write( buffer, size );
        __LA_SSIZE_T len = archive_write_data( dst, buffer, size );
        if( len != static_cast< __LA_SSIZE_T >( size ) )
            AbortArchive( log, dst );
    }
}

void TransferArchive( cpplog::BaseLogger& log, Archive* dst, Archive* src, const Path& root,
                      const Packer_ABC::T_Predicate& predicate, bool pack, io::Writer_ABC* writer )
{
    const size_t diff = root.generic_wstring().size();
    boost::shared_ptr< ArchiveEntry > ptr( archive_entry_new(), archive_entry_free );
    Path next;
    for( ;; )
    {
        ArchiveEntry* entry = ptr.get();
        int err = archive_read_next_header2( src, entry );
        if( err == ARCHIVE_EOF )
            break;
        if( err != ARCHIVE_OK )
            CheckArchiveCode( log, src, err );

        if( pack )
        {
            next = StripPathPrefix( archive_entry_pathname_w( entry ), diff );
            if( predicate && !predicate( next ) )
                continue;
            archive_read_disk_descend( src );
            if( next.empty() )
                continue;
        }
        else
        {
            const auto name = archive_entry_pathname_w( entry );
            if( !name )
                throw std::runtime_error( "missing entry path name" );
            next = root / name;
        }

        archive_entry_copy_pathname_w( entry, next.wstring().c_str() );
        err = archive_write_header( dst, entry );
        if( err != ARCHIVE_OK )
            CheckArchiveCode( log, dst, err );
        CopyBlocks( log, src, dst, writer );
        err = archive_write_finish_entry( dst );
        if( err != ARCHIVE_OK )
            CheckArchiveCode( log, dst, err );
    }
}

struct Unpacker : public Unpacker_ABC
{
    Unpacker( cpplog::BaseLogger& log, const Path& output, io::Reader_ABC& src, io::Writer_ABC* dst )
        : log_   ( log )
        , output_( output )
        , src_   ( src )
        , dst_   ( dst )
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
        Unpacker& it = *reinterpret_cast< Unpacker* >( userdata );
        try
        {
            void* dst = &it.buffer_[0];
            *buffer = dst;
            return it.src_.Read( dst, it.buffer_.size() );
        }
        catch( const std::exception& err )
        {
            LOG_ERROR( it.log_ ) << "[archive] " << err.what();
        }
        catch( ... )
        {
            LOG_ERROR( it.log_ ) << "[archive] unexpected error during read";
        }
        return -1;
    }

    void Unpack()
    {
        boost::shared_ptr< Archive > src( archive_read_new(), archive_read_free );
        archive_read_support_compression_all( src.get() );
        archive_read_support_filter_all( src.get() );
        archive_read_support_format_all( src.get() );
        archive_read_set_read_callback( src.get(), &Unpacker::Read );
        archive_read_set_callback_data( src.get(), this );
        int err = archive_read_set_options( src.get(), "hdrcharset=UTF-8" );
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( src.get() ) );
        err = archive_read_open1( src.get() );
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( src.get() ) );

        boost::shared_ptr< Archive > dst( archive_write_disk_new(), archive_write_free );
        int flags = ARCHIVE_EXTRACT_TIME
              | ARCHIVE_EXTRACT_PERM
              | ARCHIVE_EXTRACT_ACL
              | ARCHIVE_EXTRACT_FFLAGS;
        archive_write_disk_set_options( dst.get(), flags );
        archive_write_disk_set_standard_lookup( dst.get() );
        TransferArchive( log_, dst.get(), src.get(), output_, Packer_ABC::T_Predicate(), false, dst_ );
    }

    cpplog::BaseLogger& log_;
    const Path output_;
    io::Reader_ABC& src_;
    io::Writer_ABC* dst_;
    std::vector< char > buffer_;
};
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Unpack
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
FileSystem_ABC::T_Unpacker FileSystem::Unpack( const Path& output, io::Reader_ABC& src, io::Writer_ABC* dst ) const
{
    return boost::make_shared< Unpacker >( boost::ref( log_ ), output, boost::ref( src ), dst );
}

namespace
{
struct Packer : public Packer_ABC
{
    Packer( cpplog::BaseLogger& log, io::Writer_ABC& writer, ArchiveFormat fmt )
        : log_   ( log )
        , writer_( writer )
        , dst_   ( archive_write_new(), archive_write_free )
    {
        switch( fmt )
        {
            default:
                throw std::runtime_error( "unsupported archive format" );

            case runtime::ARCHIVE_FMT_ZIP:
                archive_write_set_format_zip( dst_.get() );
                break;

            case runtime::ARCHIVE_FMT_TAR_GZ:
                archive_write_set_format_gnutar( dst_.get() );
                archive_write_add_filter_gzip( dst_.get() );
                break;
        }
        int err = archive_write_set_options( dst_.get(), "hdrcharset=UTF-8");
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( dst_.get() ) );
        archive_write_set_bytes_in_last_block( dst_.get(), 1 );
        err = archive_write_open( dst_.get(), this, Packer::Dummy, Packer::Write, Packer::Dummy );
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( dst_.get() ) );
    }

    ~Packer()
    {
        // NOTHING
    }

    static int Dummy( Archive* /*arc*/, void* /*userdata*/ )
    {
        return ARCHIVE_OK;
    }

    static __LA_SSIZE_T Write( Archive* /*arc*/, void* userdata, const void* data, size_t size )
    {
        Packer& it = *reinterpret_cast< Packer* >( userdata );
        try
        {
            return it.writer_.Write( data, size );
        }
        catch( const std::exception& err )
        {
            LOG_ERROR( it.log_ ) << "[archive] " << err.what();
        }
        catch( ... )
        {
            LOG_ERROR( it.log_ ) << "[archive] unexpected error during write";
        }
        return -1;
    }

    void Pack( const Path& input, const T_Predicate& predicate )
    {
        boost::shared_ptr< Archive > src( archive_read_disk_new(), archive_read_free );
        int err = archive_read_disk_open_w( src.get(), input.generic_wstring().c_str() );
        if( err != ARCHIVE_OK )
            throw std::runtime_error( archive_error_string( src.get() ) );
        TransferArchive( log_, dst_.get(), src.get(), input, predicate, true, 0 );
    }

    void Copy( const void* fdata, size_t fsize )
    {
        const char* data = reinterpret_cast< const char* >( fdata );
        const __LA_SSIZE_T size = static_cast< __LA_SSIZE_T >( fsize );
        __LA_SSIZE_T fill = 0;
        while( fill < size )
        {
            const __LA_SSIZE_T len = archive_write_data( dst_.get(), data + fill, size - fill );
            if( len <= 0 )
                AbortArchive( log_, dst_.get() );
            fill += len;
        }
    }

    void PackEntry( const Path& file, const void* data, size_t size )
    {
        boost::shared_ptr< ArchiveEntry > ptr( archive_entry_new(), archive_entry_free );
        ArchiveEntry* entry = ptr.get();
        archive_entry_copy_pathname_w( entry, file.wstring().c_str() );
        archive_entry_set_filetype( entry, AE_IFREG );
        archive_entry_set_size( entry, size );
        archive_write_header( dst_.get(), entry );
        Copy( data, size );
        archive_write_finish_entry( dst_.get() );
    }

    cpplog::BaseLogger& log_;
    io::Writer_ABC& writer_;
    boost::shared_ptr< Archive > dst_;
};

const size_t buffer_size = 1<<18;

struct GzipWriter : public io::Writer_ABC
{
    GzipWriter( io::Writer_ABC& writer )
        : writer_( writer )
        , output_( buffer_size )
    {
        defstream.zalloc = Z_NULL;
        defstream.zfree = Z_NULL;
        defstream.opaque = Z_NULL;
        deflateInit( &defstream, Z_DEFAULT_COMPRESSION );
    }

    ~GzipWriter()
    {
        Write( 0, 0, Z_FINISH );
        deflateEnd( &defstream );
    }

    void Write( const void* data, size_t size, int flags )
    {
        const char* ptr = reinterpret_cast< const char* >( data );
        const char* end = ptr + size;
        bool last = !!( flags & Z_FINISH );
        while( ptr < end || last )
        {
            defstream.next_in = (Bytef*) ptr;
            defstream.avail_in = static_cast< uInt >( end - ptr );

            defstream.next_out = (Bytef*) &output_[0];
            defstream.avail_out = static_cast< uInt >(output_.size() );

            deflate(&defstream, flags);

            const size_t chunk = output_.size() - defstream.avail_out;
            if( chunk )
                writer_.Write( &output_[0], chunk );

            ptr = end - defstream.avail_in;
            last = false;
        }
    }

    virtual int Write( const void* data, size_t size )
    {
        Write( data, size, Z_NO_FLUSH );
        return static_cast< int >( size );
    }

    std::vector< char > output_;
    io::Writer_ABC& writer_;
    z_stream defstream;
};

}

// -----------------------------------------------------------------------------
// Name: FileSystem::Pack
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
FileSystem_ABC::T_Packer FileSystem::Pack( io::Writer_ABC& dst, ArchiveFormat fmt ) const
{
    return boost::make_shared< Packer >( boost::ref( log_ ), boost::ref( dst ), fmt );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakeGzipFilter
// Created: LGY 2013-08-01
// -----------------------------------------------------------------------------
FileSystem_ABC::T_Writer FileSystem::MakeGzipFilter( io::Writer_ABC& dst ) const
{
    return boost::shared_ptr< GzipWriter >( new GzipWriter( boost::ref( dst ) ) );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Checksum
// Created: BAX 2012-05-23
// -----------------------------------------------------------------------------
std::string FileSystem::Checksum( const Path& root, const FileSystem_ABC::T_Predicate& predicate, size_t& read ) const
{
    read = 0;
    boost::crc_32_type sum;
    std::vector< char > buffer( UINT16_MAX );
    for( boost::filesystem::recursive_directory_iterator it( root ); it != boost::filesystem::recursive_directory_iterator(); ++it )
    {
        if( !boost::filesystem::is_regular_file( it.status() ) )
            continue;
        const Path path = it->path();
        if( predicate && !predicate( path ) )
            continue;
#ifdef _MSC_VER
        // msvc does not support utf-8 strings
        std::ifstream in( path.wstring(), std::ifstream::binary );
#else
        std::ifstream in( Utf8( path ).c_str(), std::ifstream::binary );
#endif
        if( in.bad() )
        {
            LOG_ERROR( log_ ) << "[file] Unable to checksum " << path.string();
            return std::string();
        }
        while( in.good() )
        {
            in.read( &buffer[0], buffer.size() );
            const size_t next = static_cast< size_t >( in.gcount() );
            sum.process_bytes( &buffer[0], next );
            read += next;
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

// -----------------------------------------------------------------------------
// Name: FileSystem::GetDirectorySize
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
size_t FileSystem::GetDirectorySize( const Path& root ) const
{
    size_t sum = 0;
    try
    {
        if( !boost::filesystem::is_directory( root ) )
            return 0;
        for( boost::filesystem::recursive_directory_iterator it( root ); it != boost::filesystem::recursive_directory_iterator(); ++it )
            if( boost::filesystem::is_regular_file( it.status() ) )
                sum += static_cast< size_t >( boost::filesystem::file_size( it->path() ) );
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log_ ) << "[file] " << err.what();
        LOG_ERROR( log_ ) << "[file] Unable to size directory " << root.string();
    }
    return sum;
}

std::string FileSystem::ReadAll( io::Reader_ABC& input ) const
{
    std::string res;
    std::vector< char > buffer( 1<<12 );
    int len = 0;
    while( (len = input.Read( &buffer[0], buffer.size() ) ) > 0 )
        res.append( &buffer[0], len );
    return res;
}
