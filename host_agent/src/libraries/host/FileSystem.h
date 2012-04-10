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

namespace host
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
    virtual bool IsFile( const boost::filesystem::path& path ) const;
    virtual bool IsDirectory( const boost::filesystem::path& path ) const;
    virtual bool Exists( const boost::filesystem::path& path ) const;
    virtual void CopyDirectory( const boost::filesystem::path& dst, const boost::filesystem::path& src ) const;
    virtual void CopyFile( const boost::filesystem::path& dst, const boost::filesystem::path& src ) const;
    virtual void CreateDirectory( const boost::filesystem::path& path ) const;
    virtual void Remove( const boost::filesystem::path& path ) const;
    virtual void WriteFile( const boost::filesystem::path& path, const std::string& content ) const;
    virtual std::string ReadFile( const boost::filesystem::path& path ) const;
    virtual std::vector< boost::filesystem::path > Glob( const boost::filesystem::path& path, const std::wstring& name ) const;
    //@}

private:
    mutable cpplog::BaseLogger& log_;
};

}

#endif // FILE_SYSTEM_H
