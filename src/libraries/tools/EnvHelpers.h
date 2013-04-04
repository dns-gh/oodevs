// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_ENVHELPERS_H
#define TOOLS_ENVHELPERS_H

#include <tools/Path.h>
#include <vector>
#include <string>

namespace tools
{

// Parse a $PATH like input string and expand environment variables components
// if necessary. If checkDir is true, also filter out components which do not
// resolve to an existing directory.
std::vector< tools::Path > ExpandEnvPath( const std::wstring& path, bool checkDir );

// Like the previous version with "path" defaulting to getenv($PATH).
std::vector< tools::Path > ExpandEnvPath( bool checkDir );

} // namespace tools

#endif  // TOOLS_ENVHELPERS_H
