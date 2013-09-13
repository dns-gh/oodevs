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
#include <tools/Exception.h>
#include <ctime>

#pragma warning( disable: 4996 ) // This function or variable may be unsafe

using namespace dispatcher;

Log::Log( const tools::Path& filename, bool sizeInBytes )
    : tools::FileLog( filename )
    , s_          ( filename )
    , sizeInBytes_( sizeInBytes )
{
    if( ! s_ )
        throw MASA_EXCEPTION( "Failed to open log file '" + filename.ToUTF8() + "' for writing" );
}

std::size_t Log::Write( const std::string& line )
{
    const std::string time = GetTime();
    s_ << "[" << time << "] " << line << std::endl;
    return sizeInBytes_ ? time.size() + line.size() + 3 : 1;
}

std::string Log::GetTime() const
{
    char buffer[256];
    std::time_t t = time( 0 );
    std::strftime( buffer, sizeof( buffer ), "%H:%M:%S", std::localtime( &t ) );
    return buffer;
}

void Log::Close()
{
    s_.close();
}
