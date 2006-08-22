// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PathTools.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

namespace kernel
{
    namespace path_tools
    {

// -----------------------------------------------------------------------------
// Name: BuildChildPath
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
std::string BuildChildPath( const std::string& parent, const std::string& child )
{
    bfs::path parentPath( parent.c_str(), bfs::native );
    bfs::path childPath( child.c_str(), bfs::native );
    return ( parentPath.branch_path() / childPath ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: BuildWorkingDirectoryPath
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
std::string BuildWorkingDirectoryPath( const std::string& file )
{
    bfs::path cwd( ".", bfs::native );
    return ( cwd.branch_path() / file ).native_file_string();
}

    }
}