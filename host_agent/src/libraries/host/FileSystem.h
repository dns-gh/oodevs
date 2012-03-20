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
             FileSystem();
    virtual ~FileSystem();
    //@}

    //! @name Methods
    //@{
    virtual bool IsFile( const boost::filesystem::wpath& path ) const;
    virtual bool IsDirectory( const boost::filesystem::wpath& path ) const;
    virtual bool Exists( const boost::filesystem::wpath& path ) const;
    virtual void Copy( const boost::filesystem::wpath& dst, const boost::filesystem::wpath& src ) const;
    virtual void CreateDirectory( const boost::filesystem::wpath& path ) const;
    virtual void Remove( const boost::filesystem::wpath& path ) const;
    virtual void WriteFile( const boost::filesystem::wpath& path, const std::string& content ) const;
    //@}
};

}

#endif // FILE_SYSTEM_H
