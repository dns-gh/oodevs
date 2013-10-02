// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Log.h"
#include "RotatingLog.h"
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>

using namespace tools;

Log::Log( const Path& filename, std::size_t files, std::size_t size, bool truncate, bool sizeInBytes )
    : active_     ( files > 0 )
    , sizeInBytes_( sizeInBytes )
    , log_        ( new RotatingLog( *this, filename, files, size, truncate ) )
{
    // NOTHING
}

Log::~Log()
{
    // NOTHING
}

void Log::DoWrite( const std::string& s )
{
    log_->Write( s );
}

std::size_t Log::Write( std::ostream& os, const std::string& line )
{
    os << line << std::flush;
    return sizeInBytes_ ? line.size() : 1;
}

std::streamoff Log::ComputeSize( const Path& filename ) const
{
    if( sizeInBytes_ )
        return filename.FileSize();
    Ifstream file( filename );
    return std::count( std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >(), '\n');
}
