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
#include <boost/shared_ptr.hpp>
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
    typedef boost::filesystem3::path Path;
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

    //! @name Public helpers
    //@{
    typedef boost::shared_ptr< Unpacker_ABC > T_Unpacker;
    //@}

    //! @name Methods
    //@{
    virtual bool IsFile( const Path& path ) const = 0;
    virtual bool IsDirectory( const Path& path ) const = 0;
    virtual bool Exists( const Path& path ) const = 0;
    virtual void CopyDirectory( const Path& dst, const Path& src ) const = 0;
    virtual void CopyFile( const Path& dst, const Path& src ) const = 0;
    virtual void MakeDirectory( const Path& path ) const = 0;
    virtual void Remove( const Path& path ) const = 0;
    virtual void WriteFile( const Path& path, const std::string& content ) const = 0;
    virtual std::string ReadFile( const Path& path ) const = 0;
    virtual std::vector< Path > Glob( const Path& path, const std::wstring& name ) const = 0;
    virtual std::vector< Path > Walk( const Path& path, bool recurse ) const = 0;
    virtual T_Unpacker Unpack( const Path& output, std::istream& src ) const = 0;
    virtual std::string Checksum( const Path& root, const Path& exclude ) const = 0;
    //@}
};

}

#endif // FILE_SYSTEM_ABC_H
