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
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace mission_tester;
namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
Logger::Logger( std::ostream& os )
    : os_( os )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger::Created
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const std::string& orderType )
{
    missions_[target.GetId()] = std::pair< unsigned int, std::string >( mission.GetId(), mission.GetName() );
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() ) + "] >>> [U" 
         + boost::lexical_cast< std::string >( target.GetId() ) + "][M"
         + boost::lexical_cast< std::string >( mission.GetId() ) + "] " + orderType
         + " '" + mission.GetName() + "' sent." );
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionCreated
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    Created( target, mission, "mission" );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderCreated
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
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
void Logger::MissionAcknowledged( const sword::Tasker& tasker )
{
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() ) + "] <<< [U" 
         + boost::lexical_cast< std::string >( TaskerToId( tasker ) ) + "][M"
         + boost::lexical_cast< std::string >( missions_[ TaskerToId( tasker ) ].first ) + "] Mission '"
         + missions_[ TaskerToId( tasker ) ].second + "' received." );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderAcknowledged
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderAcknowledged( const sword::Tasker& tasker )
{
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() ) + "] <<< [U" 
         + boost::lexical_cast< std::string >( TaskerToId( tasker ) ) + "][M" 
         + boost::lexical_cast< std::string >( missions_[ TaskerToId( tasker ) ].first )
         + "] FragOrder '" + missions_[ TaskerToId( tasker ) ].second + "' received." );
}

namespace
{
    std::string ErrorAck( const sword::Tasker& tasker, const std::string& orderType )
    {
        std::string result( "[" + bpt::to_simple_string( bpt::second_clock::local_time() ) + "] " );
        if( tasker.has_unit() )
            return result + "<<< " + orderType + " ERROR ack for unit " + boost::lexical_cast< std::string >( tasker.unit().id() );
        else if( tasker.has_automat() )
            return result + "<<< " + orderType + " ERROR ack for automat " + boost::lexical_cast< std::string >( tasker.automat().id() );
        else if( tasker.has_crowd() )
            return result + "<<< " + orderType + " ERROR ack for crowd " + boost::lexical_cast< std::string >( tasker.crowd().id() );
        throw std::runtime_error( __FUNCTION__ " unknown unit type" );
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionErrorAck
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Logger::MissionErrorAck( const sword::Tasker& tasker )
{
    Write( ErrorAck( tasker, "Mission" ) );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderErrorAck
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void Logger::FragOrderErrorAck( const sword::Tasker& tasker )
{
    Write( ErrorAck( tasker, "FragOrder" ) );
}

// -----------------------------------------------------------------------------
// Name: Logger::ConnectionSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::ConnectionSucceeded( const std::string& endpoint )
{
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() )
         + "] <<< Connection succeeded at " + endpoint );
}

// -----------------------------------------------------------------------------
// Name: Logger::AuthenticationSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::AuthenticationSucceeded( const std::string& profile )
{
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() )
         + "] <<< Authentication succeeded with profile '" + profile + "'" );
}

// -----------------------------------------------------------------------------
// Name: Logger::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Logger::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter )
{
    Write( "[" + bpt::to_simple_string( bpt::second_clock::local_time() )
         + "] war [U" + boost::lexical_cast< std::string >( target.GetId() ) + "][M" + boost::lexical_cast< std::string >( mission.GetId() )
         + "] paramType unknown '" + parameter.GetType() + "'" );
}

// -----------------------------------------------------------------------------
// Name: Logger::Write
// Created: PHC 2011-05-23
// -----------------------------------------------------------------------------
void Logger::Write( const std::string& input )
{
    os_ << input << std::endl;
}
