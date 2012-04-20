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

#include <cpplog/cpplog.hpp>
#include <runtime/Utf8.h>

using namespace host;

// -----------------------------------------------------------------------------
// Name: FileSystem::FileSystem
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
FileSystem::FileSystem( cpplog::BaseLogger& log )
    : log_( log )
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
bool FileSystem::IsFile( const boost::filesystem::path& path ) const
{
    return boost::filesystem::is_regular_file( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::IsDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::IsDirectory( const boost::filesystem::path& path ) const
{
    return boost::filesystem::is_directory( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Exists
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
bool FileSystem::Exists( const boost::filesystem::path& path ) const
{
    return boost::filesystem::exists( path );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::CopyDirectory( const boost::filesystem::path& src, const boost::filesystem::path& dst ) const
{
    // TODO Use wrecursive_directory_iterator
    if( IsFile( src ) )
        return boost::filesystem::copy_file( src, dst / src.filename() );
    else if( !IsDirectory( src ) )
        return;
    const boost::filesystem::path sub = dst / src.filename();
    if( !boost::filesystem::create_directory( sub ) )
        throw std::runtime_error( "unable to create " + sub.string() );
    for( boost::filesystem::directory_iterator it( src ); it != boost::filesystem::directory_iterator(); ++it )
        CopyDirectory( *it, sub );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::CopyFile
// Created: BAX 2012-03-29
// -----------------------------------------------------------------------------
void FileSystem::CopyFile( const boost::filesystem::path& src, const boost::filesystem::path& dst ) const
{
    boost::filesystem::copy_file( src, dst / src.filename() );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::MakeDirectory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::MakeDirectory( const boost::filesystem::path& path ) const
{
    boost::filesystem::create_directories( path );
}

namespace
{
    std::string TryRemove( const boost::filesystem::path& path )
    {
        try
        {
            boost::filesystem::remove_all( path );
            return std::string();
        }
        catch( std::exception& err )
        {
            return err.what();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Remove
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
void FileSystem::Remove( const boost::filesystem::path& path ) const
{
    std::string reply = TryRemove( path );
    if( !reply.empty() )
        reply = TryRemove( path );
    if( !reply.empty() )
        LOG_ERROR( log_ ) << "[file] Unable to remove path " + path.string() + ", " + reply;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::WriteFile
// Created: BAX 2012-03-20
// -----------------------------------------------------------------------------
void FileSystem::WriteFile( const boost::filesystem::path& path, const std::string& content ) const
{
    boost::filesystem::ofstream( path ) << content;
}

// -----------------------------------------------------------------------------
// Name: FileSystem::ReadFile
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::string FileSystem::ReadFile( const boost::filesystem::path& path ) const
{
    boost::filesystem::ifstream ifs( path );
    return std::string( std::istreambuf_iterator< char >( ifs ), std::istreambuf_iterator< char >() );
}

// -----------------------------------------------------------------------------
// Name: FileSystem::Glob
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
std::vector< boost::filesystem::path > FileSystem::Glob( const boost::filesystem::path& path, const std::wstring& name ) const
{
    std::vector< boost::filesystem::path > paths;
    if( IsDirectory( path ) )
        for( boost::filesystem::recursive_directory_iterator it( path ); it != boost::filesystem::recursive_directory_iterator(); ++it )
            if( IsFile( *it ) && it->path().filename() == name )
                paths.push_back( *it );
    return paths;
}