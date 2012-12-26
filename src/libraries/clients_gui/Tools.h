// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_TOOLS_H
#define CLIENTS_GUI_TOOLS_H

#include "clients_kernel/Tools.h"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace gui
{

typedef bool (*ListDirValidator)( const bfs::path& );

// Make a recursive traversal of "base" subtree and return the list of
// paths matched by the validation function. Returned path a relative to
// base and their components separate by slashes.
QStringList ListDirectories( const std::string& base, ListDirValidator v );

bool IsPropagationDir( const bfs::path& dir );

std::string BuildPropagationDir( const std::string& root, const std::string& path );

}  // namespace gui

#endif  // CLIENTS_GUI_TOOLS_H
