// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef HOST_ERROR_H
#define HOST_ERROR_H

#include <string>

namespace tools
{
    class Path;
}

namespace runtime
{
    struct FileSystem_ABC;
}

namespace host
{
    std::string GetLastError( const runtime::FileSystem_ABC& fs, const tools::Path& output );
}

#endif // HOST_ERROR_H
