// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "FileLogger.h"
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: FileLogger constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
FileLogger::FileLogger( const boost::filesystem::path& path, const std::string& extension/* = ".log"*/ )
    : lines_    ( 0 )
    , files_    ( 0 )
    , path_     ( path )
    , extension_( extension )
    , os_       ( 0 )
    , logger_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLogger destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
FileLogger::~FileLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLogger::UpdateLogger
// Created: PHC 2011-05-24
// -----------------------------------------------------------------------------
void FileLogger::UpdateLogger()
{
    if( !logger_.get() || lines_ >= 100000 )
    {
        lines_ = 0;
        ++files_;
        std::string file( ( path_ / ( "tester_" + boost::lexical_cast< std::string >( files_ ) + extension_ ) ).string() );
        os_.reset( new std::ofstream( file.c_str() ) );
        logger_.reset( new Logger( *os_ ) );
        WriteHeader();
    }
    else
        ++lines_;
}

// -----------------------------------------------------------------------------
// Name: FileLogger::Write
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::Write( const std::string& input )
{
    UpdateLogger();
    logger_->Write( input );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::WriteHeader
// Created: PHC 2011-05-25
// -----------------------------------------------------------------------------
void FileLogger::WriteHeader()
{
    logger_->Write( "### File created on " + boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) + " ###" );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::MissionCreated
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    UpdateLogger();
    logger_->MissionCreated( target, mission );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::FragOrderCreated
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    UpdateLogger();
    logger_->FragOrderCreated( target, mission );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::MissionAcknowledged
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::MissionAcknowledged( const sword::Tasker& tasker )
{
    UpdateLogger();
    logger_->MissionAcknowledged( tasker );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::FragOrderAcknowledged
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::FragOrderAcknowledged( const sword::Tasker& tasker )
{
    UpdateLogger();
    logger_->FragOrderAcknowledged( tasker );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::MissionErrorAck
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::MissionErrorAck( const sword::Tasker& tasker )
{
    UpdateLogger();
    logger_->MissionErrorAck( tasker );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::FragOrderErrorAck
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::FragOrderErrorAck( const sword::Tasker& tasker )
{
    UpdateLogger();
    logger_->FragOrderErrorAck( tasker );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::ConnectionSucceeded
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::ConnectionSucceeded( const std::string& endpoint )
{
    UpdateLogger();
    logger_->ConnectionSucceeded( endpoint );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::AuthenticationSucceeded
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::AuthenticationSucceeded( const std::string& profile )
{
    UpdateLogger();
    logger_->AuthenticationSucceeded( profile );
}

// -----------------------------------------------------------------------------
// Name: FileLogger::ParameterCreationFailed
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void FileLogger::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission,
                                          const kernel::OrderParameter& parameter )
{
    UpdateLogger();
    logger_->ParameterCreationFailed( target, mission, parameter );
}
