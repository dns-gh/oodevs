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
#include "Config.h"
#include <ctime>

using namespace dispatcher;

Log::Log( const Config& config )
    : dispatcher::Log_ABC( config.GetDispatcherProtobufLogFiles() > 0 )
    , log_(
        config.BuildSessionChildFile( "Protobuf.log" ), config.GetDispatcherProtobufLogFiles(),
        config.GetDispatcherProtobufLogSize(), true, config.IsDispatcherProtobufLogInBytes() )
{
    // NOTHING
}

Log::~Log()
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

void Log::DoWrite( const std::string& line )
{
    std::stringstream s;
    const std::string time = GetTime();
    s << "[" << time << "] " << line << std::endl;
    log_.Write( line );
}
