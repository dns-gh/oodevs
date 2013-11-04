// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace boost
{
namespace filesystem
{
    class path;
}
}

namespace runtime
{
    struct FileSystem_ABC;
}

namespace host
{
    typedef boost::filesystem::path Path;

    std::string GetLastError( const runtime::FileSystem_ABC& fs, const Path& output );
}

#endif // ERROR_H
