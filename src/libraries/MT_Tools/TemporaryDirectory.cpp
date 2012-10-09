// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "TemporaryDirectory.h"
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <cstdlib>

namespace bfs = boost::filesystem;

namespace
{

std::string NormalizePath( std::string path )
{
    for( size_t i = 0; i != path.size(); ++i )
    {
        if( path[i] == '\\')
            path[i] = '/';
    }
    return path;
}

}  // namespace


TemporaryDirectory::TemporaryDirectory( const std::string& namePrefix, const std::string& basePath )
{
    const std::string suffix = boost::lexical_cast< std::string >( rand() );
    const std::string name = namePrefix + suffix;
    const bfs::path path = bfs::path( basePath ) / name;
    if( bfs::exists( path ) )
        bfs::remove_all( path );
    bfs::create_directories( path );
    path_ = bfs::path( NormalizePath( path.string() ) );
}

TemporaryDirectory::~TemporaryDirectory()
{
    try
    {
        bfs::remove_all( path_ );
    }
    catch( const bfs::filesystem_error& )
    {
    }
}

const bfs::path& TemporaryDirectory::path() const
{
    return path_;
}

void TemporaryDirectory::ListDir( std::vector< std::string >& files ) const
{
    files.clear();
    bfs::recursive_directory_iterator end;
    for( bfs::recursive_directory_iterator entry( path() ) ; entry != end ; ++entry )
    {
        if( bfs::is_regular_file( entry->status() ) )
            files.push_back( NormalizePath( entry->path().string() ) );
    }
    std::sort( files.begin(), files.end() );
}