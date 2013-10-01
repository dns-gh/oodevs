// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FileLogger.h"
#include "Config.h"
#include "tools/RotatingLog.h"
#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FileLogger constructor
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
FileLogger::FileLogger( const tools::Path& filename, const Config& config )
    : log_( new tools::RotatingLog( *this, filename, config.GetDispatcherLogFiles(), config.GetDispatcherLogSize(), ! config.HasCheckpoint() ) )
    , sizeInBytes_( config.IsDispatcherLogInBytes() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLogger destructor
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
FileLogger::~FileLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLogger::LogInfo
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void FileLogger::LogInfo( const std::string& message )
{
    LogMessage( "Info", message );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::LogError
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void FileLogger::LogError( const std::string& message )
{
    LogMessage( "Error", message );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::LogWarning
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void FileLogger::LogWarning( const std::string& message )
{
    LogMessage( "Warning", message );
}

namespace
{
    std::string Timestamp()
    {
        namespace bpt = boost::posix_time;
        return bpt::to_simple_string( bpt::second_clock::local_time().time_of_day() );
    }
}

// -----------------------------------------------------------------------------
// Name: FileLogger::LogMessage
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void FileLogger::LogMessage( const std::string& severity, const std::string& message )
{
    std::stringstream s;
    s << "[" << Timestamp() << "] " << severity << " - " << message;
    boost::mutex::scoped_lock locker( mutex_ );
    log_->Write( s.str() );
}

std::size_t FileLogger::Write( std::ostream& os, const std::string& line )
{
    os << line << std::flush;
    return sizeInBytes_ ? line.size() : 1;
}

std::streamoff FileLogger::ComputeSize( const tools::Path& filename ) const
{
    if( sizeInBytes_ )
        return filename.FileSize();
    tools::Ifstream file( filename );
    return std::count( std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >(), '\n');
}
