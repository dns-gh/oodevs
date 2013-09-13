// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef tools_FileLog_h
#define tools_FileLog_h

#include "Log_ABC.h"
#include <tools/Path.h>

namespace tools
{
    class Path;

// =============================================================================
/** @class  FileLog
    @brief  Log base class factorizing behavior to common file logs.
*/
// Created: MCO 2013-09-12
// =============================================================================
class FileLog : public Log_ABC
{
public:
    FileLog( const Path& filename )
        : filename_( filename )
    {}

    virtual void Rename( const tools::Path& filename )
    {
        Close();
        filename_.Rename( filename );
        filename_ = filename;
    }

    virtual void Delete()
    {
        Close();
        filename_.Remove();
    }

private:
    virtual void Close() = 0;

private:
    Path filename_;
};

}

#endif // tools_FileLog_h
