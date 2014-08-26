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
namespace filesystem
{
    class path;
}
}

namespace io
{
    struct Reader_ABC;
    struct Writer_ABC;
}

namespace runtime
{
    typedef boost::filesystem::path Path;
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
    typedef std::function< bool( const Path& ) > T_Predicate;
    //@}

    //! @name Methods
    //@{
    virtual void Pack( const Path& input, const T_Predicate& predicate ) = 0;
    virtual void PackEntry( const Path& file, const void* data, size_t size ) = 0;
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
    typedef std::function< bool( const Path& ) > T_Predicate;
    //@}

    //! @name Methods
    //@{
    virtual bool        IsFile( const Path& path ) const = 0;
    virtual bool        IsDirectory( const Path& path ) const = 0;
    virtual bool        Exists( const Path& path ) const = 0;
    virtual void        CopyDirectory( const Path& dst, const Path& src ) const = 0;
    virtual bool        CopyFile( const Path& dst, const Path& src ) const = 0;
    virtual void        MakePaths( const Path& path ) const = 0;
    virtual bool        MakePath( const Path& path ) const = 0;
    virtual bool        Remove( const Path& path ) const = 0;
    virtual bool        Rename( const Path& src, const Path& dst ) const = 0;
    virtual bool        WriteFile( const Path& path, const std::string& content ) const = 0;
    virtual std::string ReadFile( const Path& path ) const = 0;
    virtual void        LimitedReadFile( io::Writer_ABC& sink, const Path& path, int limit ) const = 0;
    virtual void        Walk( const Path& path, bool recurse, const T_Predicate& predicate ) const = 0;
    virtual T_Unpacker  Unpack( const Path& output, io::Reader_ABC& src, io::Writer_ABC* dst ) const = 0;
    virtual T_Packer    Pack( io::Writer_ABC& dst, ArchiveFormat fmt ) const = 0;
    virtual T_Writer    MakeGzipFilter( io::Writer_ABC& writer ) const = 0;
    virtual std::string Checksum( const Path& root, const T_Predicate& predicate, size_t& read ) const = 0;
    virtual Path        MakeAnyPath( const Path& root ) const = 0;
    virtual std::time_t GetLastWrite( const Path& file ) const = 0;
    virtual size_t      GetDirectorySize( const Path& dir ) const = 0;
    virtual std::string ReadAll( io::Reader_ABC& input ) const = 0;
    //@}
};
}

#endif // FILE_SYSTEM_ABC_H
