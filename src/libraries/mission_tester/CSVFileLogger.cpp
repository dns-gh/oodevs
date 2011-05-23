// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "CSVFileLogger.h"

#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/protocol.h"
#include <iostream>
#pragma warning( push, 0 )
#include <qstring.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: CSVFileLogger constructor
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
CSVFileLogger::CSVFileLogger( const std::string& filename )
    : FileLogger( filename + ".csv" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger destructor
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
CSVFileLogger::~CSVFileLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger::Created
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void CSVFileLogger::Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const std::string& orderType )
{
    os_ << boost::posix_time::second_clock::local_time() << ";"
        << "out;"
        << target.GetId() << ";"
        << mission.GetId()<< ";"
        << orderType << " '" << mission.GetName() << "' sent." << std::endl;
    missions_[target.GetId()] = std::pair< unsigned int, std::string >( mission.GetId(), mission.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionCreated
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    Created( target, mission, "mission" );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderCreated
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    Created( target, mission, "fragorder" );
}

namespace
{
    unsigned int TaskerToId( const sword::Tasker& tasker )
    {
        if( tasker.has_automat() )
            return tasker.automat().id();
        if( tasker.has_unit() )
            return tasker.unit().id();
        if( tasker.has_crowd() )
            return tasker.crowd().id();
        throw std::runtime_error( __FUNCTION__ " unknown tasker type" );
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionAknowledged
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionAcknowledged( const sword::Tasker& tasker )
{
    os_ << boost::posix_time::second_clock::local_time() << ";"
        << "in;"
        << TaskerToId( tasker ) << ";"
        << missions_[ TaskerToId( tasker ) ].first << ";"
        << "Mission '" << missions_[ TaskerToId( tasker ) ].second << "' received." << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderAcknowledged
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderAcknowledged( const sword::Tasker& tasker )
{
    os_ << boost::posix_time::second_clock::local_time() << ";"
        << "in;"
        << TaskerToId( tasker ) << ";"
        << missions_[ TaskerToId( tasker ) ].first << ";"
        << "FragOrder '" << missions_[ TaskerToId( tasker ) ].second << "' received." << std::endl;
}

namespace
{
    void ErrorAck( const sword::Tasker& tasker, std::ostream& os, const std::string& orderType )
    {
        os << boost::posix_time::second_clock::local_time() << ";"
           << "ERROR;";
        if( tasker.has_unit() )
            os << tasker.unit().id() << ";;";
        else if( tasker.has_automat() )
            os << tasker.automat().id() << ";;";
        else if( tasker.has_crowd() )
            os << tasker.crowd().id() << ";;";
        os << "error on " << orderType << " ack" <<std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionErrorAck
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionErrorAck( const sword::Tasker& tasker ) const
{
    ErrorAck( tasker, os_, "Mission" );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderErrorAck
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderErrorAck( const sword::Tasker& tasker ) const
{
    ErrorAck( tasker, os_, "FragOrder" );
}

// -----------------------------------------------------------------------------
// Name: Logger::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void CSVFileLogger::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const
{
    os_ << boost::posix_time::second_clock::local_time() << ";"
        << "warning;" 
        << target.GetId() << ";"
        << mission.GetId() << ";"
        << "paramType unknown '" << parameter.GetType() << "'" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger::ConnectionSucceeded
// Created: PHC 2011-05-20
// -----------------------------------------------------------------------------
void CSVFileLogger::ConnectionSucceeded( const std::string& /*endpoint*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger::AuthenticationSucceeded
// Created: PHC 2011-05-20
// -----------------------------------------------------------------------------
void CSVFileLogger::AuthenticationSucceeded( const std::string& /*profile*/ ) const
{
    os_ << "Date;Message type;Entity ID;Order ID;Info" << std::endl;
}
