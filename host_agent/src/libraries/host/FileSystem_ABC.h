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
#include <string>
#include <vector>

namespace boost
{
namespace filesystem3
{
    class path;
}
}

namespace host
{

// =============================================================================
/** @class  FileSystem_ABC
    @brief  FileSystem_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class FileSystem_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileSystem_ABC() {}
    virtual ~FileSystem_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual bool IsFile( const boost::filesystem3::path& path ) const = 0;
    virtual bool IsDirectory( const boost::filesystem3::path& path ) const = 0;
    virtual bool Exists( const boost::filesystem3::path& path ) const = 0;
    virtual void CopyDirectory( const boost::filesystem3::path& dst, const boost::filesystem3::path& src ) const = 0;
    virtual void CopyFile( const boost::filesystem3::path& dst, const boost::filesystem3::path& src ) const = 0;
    virtual void MakeDirectory( const boost::filesystem3::path& path ) const = 0;
    virtual void Remove( const boost::filesystem3::path& path ) const = 0;
    virtual void WriteFile( const boost::filesystem3::path& path, const std::string& content ) const = 0;
    virtual std::string ReadFile( const boost::filesystem3::path& path ) const = 0;
    virtual std::vector< boost::filesystem3::path > Glob( const boost::filesystem3::path& path, const std::wstring& name ) const = 0;
    virtual void Unpack( const boost::filesystem3::path& input, const boost::filesystem3::path& prefix ) const = 0;
    //@}
};

}

#endif // FILE_SYSTEM_ABC_H
