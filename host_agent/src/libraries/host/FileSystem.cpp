// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FileSystem.h"

#include <boost/filesystem/fstream.hpp>

#include <runtime/Utf8.h>

using namespace host;

// -----------------------------------------------------------------------------
// Name: FileSystem::FileSystem
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
FileSystem::FileSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileSystem::~FileSystem
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
FileSystem::~FileSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsFile
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsFile( const boost::filesystem::wpath& path ) const
{
    return boost::filesystem::is_regular_file( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsDirectory( const boost::filesystem::wpath& path ) const
{
    return boost::filesystem::is_directory( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Exists
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::Exists( const boost::filesystem::wpath& path ) const
{
    return boost::filesystem::exists( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Copy
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::Copy( const boost::filesystem::wpath& src, const boost::filesystem::wpath& dst ) const
{
    if( IsFile( src ) )
        return boost::filesystem::copy_file( src, dst / src.filename() );
    else if( !IsDirectory( src ) )
        return;
    const boost::filesystem::wpath sub = dst / src.filename();
    if( !boost::filesystem::create_directory( sub ) )
        throw std::runtime_error( "unable to create " + runtime::Utf8Convert( sub.string() ) );
    for( boost::filesystem::wdirectory_iterator it( src ); it != boost::filesystem::wdirectory_iterator(); ++it )
        Copy( *it, sub );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CreateDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::CreateDirectory( const boost::filesystem::wpath& path ) const
{
    boost::filesystem::create_directories( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Remove
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::Remove( const boost::filesystem::wpath& path ) const
{
    boost::filesystem::remove_all( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::WriteFile
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void FileSystem::WriteFile( const boost::filesystem::wpath& path, const std::string& content ) const
{
    boost::filesystem::ofstream( path ) << content;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::ReadFile
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string FileSystem::ReadFile( const boost::filesystem::wpath& path ) const
{
    std::string reply;
    boost::filesystem::ifstream( path ) >> reply;
    return reply;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Glob
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::vector< boost::filesystem::wpath > FileSystem::Glob( const boost::filesystem::wpath& path, const std::wstring& name ) const
{
    std::vector< boost::filesystem::wpath > paths;
    for( boost::filesystem::wrecursive_directory_iterator it( path ); it != boost::filesystem::wrecursive_directory_iterator(); ++it )
        if( IsFile( *it ) && it->filename() == name )
            paths.push_back( *it );
    return paths;
}