// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef FILE_SYSTEM_ABC_H
#define FILE_SYSTEM_ABC_H

#include <boost/noncopyable.hpp>
#include <ctime>
#include <functional>
#include <string>

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace io
{
    struct Reader_ABC;
    struct Writer_ABC;
}

namespace tools
{
    class Path;
}

namespace runtime
{
// =============================================================================
/** @class  Unpacker_ABC
    @brief  Unpacker_ABC interface
*/
// Created: BAX 2012-05-11
// =============================================================================
struct Unpacker_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Unpacker_ABC() {}
    virtual ~Unpacker_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual void Unpack() = 0;
    //@}
};

// =============================================================================
/** @class  Packer_ABC
    @brief  Packer_ABC interface
*/
// Created: BAX 2012-08-06
// =============================================================================
struct Packer_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Packer_ABC() {}
    virtual ~Packer_ABC() {}
    //@}

    //! @name Methods
    //@{
    typedef std::function< bool( const tools::Path& ) > T_Predicate;
    //@}

    //! @name Methods
    //@{
    virtual void Pack( const tools::Path& input, const T_Predicate& predicate ) = 0;
    virtual void PackEntry( const tools::Path& file, const void* data, size_t size ) = 0;
    //@}
};

enum ArchiveFormat
{
    ARCHIVE_FMT_ZIP,
    ARCHIVE_FMT_TAR_GZ,
    ARCHIVE_FMT_COUNT,
};

// =============================================================================
/** @class  FileSystem_ABC
    @brief  FileSystem_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
struct FileSystem_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             FileSystem_ABC() {}
    virtual ~FileSystem_ABC() {}
    //@}

    //! @name Public helpers
    //@{
    typedef boost::shared_ptr< Unpacker_ABC > T_Unpacker;
    typedef boost::shared_ptr< Packer_ABC > T_Packer;
    typedef boost::shared_ptr< io::Writer_ABC > T_Writer;
    typedef std::function< bool( const tools::Path& ) > T_Predicate;
    //@}

    //! @name Methods
    //@{
    virtual bool        IsFile( const tools::Path& path ) const = 0;
    virtual bool        IsDirectory( const tools::Path& path ) const = 0;
    virtual bool        Exists( const tools::Path& path ) const = 0;
    virtual void        CopyDirectory( const tools::Path& dst, const tools::Path& src ) const = 0;
    virtual bool        CopyFile( const tools::Path& dst, const tools::Path& src ) const = 0;
    virtual void        MakePaths( const tools::Path& path ) const = 0;
    virtual bool        MakePath( const tools::Path& path ) const = 0;
    virtual bool        Remove( const tools::Path& path ) const = 0;
    virtual bool        Rename( const tools::Path& src, const tools::Path& dst ) const = 0;
    virtual bool        WriteFile( const tools::Path& path, const std::string& content ) const = 0;
    virtual std::string ReadFile( const tools::Path& path ) const = 0;
    virtual void        LimitedReadFile( io::Writer_ABC& sink, const tools::Path& path, int limit ) const = 0;
    virtual void        Walk( const tools::Path& path, bool recurse, const T_Predicate& predicate ) const = 0;
    virtual T_Unpacker  Unpack( const tools::Path& output, io::Reader_ABC& src, io::Writer_ABC* dst ) const = 0;
    virtual T_Packer    Pack( io::Writer_ABC& dst, ArchiveFormat fmt ) const = 0;
    virtual T_Writer    MakeDeflateFilter( io::Writer_ABC& writer ) const = 0;
    virtual std::string Checksum( const tools::Path& root, const T_Predicate& predicate, size_t& read ) const = 0;
    virtual tools::Path MakeAnyPath( const tools::Path& root ) const = 0;
    virtual std::time_t GetLastWrite( const tools::Path& file ) const = 0;
    virtual size_t      GetDirectorySize( const tools::Path& dir ) const = 0;
    virtual std::string ReadAll( io::Reader_ABC& input ) const = 0;
    //@}
};
}

#endif // FILE_SYSTEM_ABC_H
