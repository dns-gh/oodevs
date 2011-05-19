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
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FileLogger constructor
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
FileLogger::FileLogger( const std::string& filename )
    : output_( filename.c_str(), std::ios::out | std::ios::trunc )
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
    boost::mutex::scoped_lock locker( mutex_ );
    output_ << "[" << Timestamp() << "] "
            << severity << " - "
            << message << std::endl;
    output_.flush();
}
