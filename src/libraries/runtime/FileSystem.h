// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "FileSystem_ABC.h"

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
// =============================================================================
/** @class  FileSystem
    @brief  FileSystem interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class FileSystem : public FileSystem_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileSystem( cpplog::BaseLogger& log );
    virtual ~FileSystem();
    //@}

    //! @name Methods
    //@{
    virtual bool        IsFile( const tools::Path& path ) const;
    virtual bool        IsDirectory( const tools::Path& path ) const;
    virtual bool        Exists( const tools::Path& path ) const;
    virtual void        CopyDirectory( const tools::Path& dst, const tools::Path& src ) const;
    virtual bool        CopyFile( const tools::Path& dst, const tools::Path& src ) const;
    virtual void        MakePaths( const tools::Path& path ) const;
    virtual bool        MakePath( const tools::Path& path ) const;
    virtual bool        Remove( const tools::Path& path ) const;
    virtual bool        Rename( const tools::Path& src, const tools::Path& dst ) const;
    virtual bool        WriteFile( const tools::Path& path, const std::string& content ) const;
    virtual std::string ReadFile( const tools::Path& path ) const;
    virtual void        LimitedReadFile( io::Writer_ABC& sink, const tools::Path& path, int limit ) const;
    virtual void        Walk( const tools::Path& path, bool recurse, const T_Predicate& predicate ) const;
    virtual T_Unpacker  Unpack( const tools::Path& output, io::Reader_ABC& src, io::Writer_ABC* dst ) const;
    virtual T_Packer    Pack( io::Writer_ABC& dst, ArchiveFormat fmt ) const;
    virtual T_Writer    MakeDeflateFilter( io::Writer_ABC& writer ) const;
    virtual std::string Checksum( const tools::Path& root, const T_Predicate& predicate, size_t& read ) const;
    virtual tools::Path MakeAnyPath( const tools::Path& root ) const;
    virtual std::time_t GetLastWrite( const tools::Path& file ) const;
    virtual size_t      GetDirectorySize( const tools::Path& dir ) const;
    virtual std::string ReadAll( io::Reader_ABC& input ) const;
    //@}

private:
    cpplog::BaseLogger& log_;
};
}

#endif // FILE_SYSTEM_H
