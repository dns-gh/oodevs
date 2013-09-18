// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Log.h"
#include <ctime>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>

using namespace dispatcher;

Log::Log( bool sizeInBytes )
    : sizeInBytes_( sizeInBytes )
{
    // NOTHING
}

namespace
{
    std::string GetTime()
    {
        char buffer[256];
        std::time_t t = time( 0 );
        std::strftime( buffer, sizeof( buffer ), "%H:%M:%S", std::localtime( &t ) );
        return buffer;
    }
}

std::size_t Log::Write( std::ostream& os, const std::string& line )
{
    const std::string time = GetTime();
    os << "[" << time << "] " << line << std::endl;
    return sizeInBytes_ ? time.size() + line.size() + 3 : 1;
}

std::streamoff Log::ComputeSize( const tools::Path& filename ) const
{
    if( sizeInBytes_ )
        return filename.FileSize();
    tools::Ifstream file( filename );
    return std::count( std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >(), '\n');
}
