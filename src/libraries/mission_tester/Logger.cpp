// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Logger.h"
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
// Name: Logger constructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
Logger::Logger( std::ostream& os )
    : os_   ( os )
    , facet_( new boost::posix_time::time_facet( "%d-%H:%M:%S" ) )
{
    std::cout.imbue( std::locale( std::cout.getloc(), facet_.get() ) );
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    // NOTHING
}

namespace
{
    void Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, std::ostream& os, const std::string& orderType )
    {
        os << "[" << boost::posix_time::second_clock::local_time() << "] >>>"
           << " [U" << target.GetId() << "]"
           << "[M" << mission.GetId()<< "] " << orderType << " '" << mission.GetName() << "' sent." << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionCreated
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission ) const
{
    Created( target, mission, os_, "mission" );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderCreated
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission ) const
{
    Created( target, mission, os_, "fragorder" );
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
void Logger::MissionAcknowledged( const sword::Tasker& tasker ) const
{
    os_ << "[" << boost::posix_time::second_clock::local_time() << "] <<<"
        << " [U" << TaskerToId( tasker ) << "]"
        << "[M ?" << "] Mission received." << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderAcknowledged
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderAcknowledged( const sword::Tasker& tasker ) const
{
    
    os_ << "[" << boost::posix_time::second_clock::local_time() << "] <<<"
        << " [U" << TaskerToId( tasker ) <<     "]"
        << "[M ?" << "] FragOrder received." << std::endl;
}

namespace
{
    void ErrorAck( const sword::Tasker& tasker, std::ostream& os, const std::string& orderType )
    {
        os << "[" << boost::posix_time::second_clock::local_time() << "] ";
        if( tasker.has_unit() )
            os << "<<< " << orderType << " ERROR ack for unit " << tasker.unit().id() << std::endl;
        else if( tasker.has_automat() )
            os << "<<< " << orderType << " ERROR ack for automat " << tasker.automat().id() << std::endl;
        else if( tasker.has_crowd() )
            os << "<<< " << orderType << " ERROR ack for crowd " << tasker.crowd().id() << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionErrorAck
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Logger::MissionErrorAck( const sword::Tasker& tasker ) const
{
    ErrorAck( tasker, os_, "Mission" );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderErrorAck
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderErrorAck( const sword::Tasker& tasker ) const
{
    ErrorAck( tasker, os_, "FragOrder" );
}

// -----------------------------------------------------------------------------
// Name: Logger::ConnectionSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::ConnectionSucceeded( const std::string& endpoint ) const
{
    os_ << "[" << boost::posix_time::second_clock::local_time()
        << "] <<< Connection succeeded at " << endpoint << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::AuthenticationSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::AuthenticationSucceeded( const std::string& profile ) const
{
    os_ << "[" << boost::posix_time::second_clock::local_time()
        << "] <<< Authentication succeeded with profile '" << profile << "'" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Logger::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const
{
    os_ << "[" << boost::posix_time::second_clock::local_time()
        << "] war [U" << target.GetId() << "][M" << mission.GetId() << "] paramType unknown '" << parameter.GetType() << "'" << std::endl;
}
