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
    virtual bool IsFile( const Path& path ) const;
    virtual bool IsDirectory( const Path& path ) const;
    virtual bool Exists( const Path& path ) const;
    virtual void CopyDirectory( const Path& dst, const Path& src ) const;
    virtual bool CopyFile( const Path& dst, const Path& src ) const;
    virtual void MakePaths( const Path& path ) const;
    virtual bool MakePath( const Path& path ) const;
    virtual bool Remove( const Path& path ) const;
    virtual bool Rename( const Path& src, const Path& dst ) const;
    virtual bool WriteFile( const Path& path, const std::string& content ) const;
    virtual std::string ReadFile( const Path& path ) const;
    virtual std::vector< Path > Glob( const Path& path, const Path& name ) const;
    virtual std::vector< Path > Walk( const Path& path, bool recurse ) const;
    virtual T_Unpacker Unpack( const Path& output, std::istream& src ) const;
    virtual std::string Checksum( const Path& root, const T_Predicate& predicate = T_Predicate() ) const;
    virtual Path MakeAnyPath( const Path& root ) const;
    virtual std::time_t GetLastWrite( const Path& file ) const;
    //@}

private:
    cpplog::BaseLogger& log_;
};

}

#endif // FILE_SYSTEM_H
