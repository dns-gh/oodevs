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
#include <QtCore/qstring.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace mission_tester;
namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: CSVFileLogger constructor
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
CSVFileLogger::CSVFileLogger( const boost::filesystem::path& path )
    : FileLogger( path, ".csv" )
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

namespace
{
    void Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const std::string& orderType
                , std::map< unsigned int, std::pair< unsigned int, std::string > >& missions, CSVFileLogger& logger )
    {
        logger.Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";out;"
                    + boost::lexical_cast< std::string >( target.GetId() ) + ";"
                    + boost::lexical_cast< std::string >( mission.GetId() ) + ";"
                    + orderType + " '" + mission.GetName() + "' sent." );
        missions[target.GetId()] = std::pair< unsigned int, std::string >( mission.GetId(), mission.GetName() );
    }
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
    std::string ErrorAck( const sword::Tasker& tasker, const std::string& orderType )
    {
        std::string result( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";ERROR;" );
        if( tasker.has_unit() )
            result += tasker.unit().id() + ";;";
        else if( tasker.has_automat() )
            result += tasker.automat().id() + ";;";
        else if( tasker.has_crowd() )
            result += tasker.crowd().id() + ";;";
        return result + "error on " + orderType + " ack";
    }
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionCreated
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    Created( target, mission, "mission", missions_, *this );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderCreated
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission )
{
    Created( target, mission, "fragorder", missions_, *this );
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionAknowledged
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionAcknowledged( const sword::Tasker& tasker )
{
    Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";in;"
         + boost::lexical_cast< std::string >( TaskerToId( tasker ) ) + ";"
         + boost::lexical_cast< std::string >( missions_[ TaskerToId( tasker ) ].first ) + ";"
         + "Mission '" + missions_[ TaskerToId( tasker ) ].second + "' received." );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderAcknowledged
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderAcknowledged( const sword::Tasker& tasker )
{
    Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";in;"
         + boost::lexical_cast< std::string >( TaskerToId( tasker ) ) + ";"
         + boost::lexical_cast< std::string >( missions_[ TaskerToId( tasker ) ].first ) + ";"
         + "FragOrder '" + missions_[ TaskerToId( tasker ) ].second + "' received." );
}

// -----------------------------------------------------------------------------
// Name: Logger::MissionErrorAck
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void CSVFileLogger::MissionErrorAck( const sword::Tasker& tasker )
{
    Write( ErrorAck( tasker, "Mission" ) );
}

// -----------------------------------------------------------------------------
// Name: Logger::FragOrderErrorAck
// Created: PHC 2011-05-19
// -----------------------------------------------------------------------------
void CSVFileLogger::FragOrderErrorAck( const sword::Tasker& tasker )
{
    ErrorAck( tasker, "FragOrder" );
}

// -----------------------------------------------------------------------------
// Name: Logger::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void CSVFileLogger::ParameterCreationFailed( const kernel::Entity_ABC& target,
                                             const kernel::OrderType& mission,
                                             const kernel::OrderParameter& parameter )
{
    Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";warning;"
         + boost::lexical_cast< std::string >( target.GetId() ) + ";"
         + boost::lexical_cast< std::string >( mission.GetId() ) + ";paramType unknown '"
         + parameter.GetType() + "'" );
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger::ConnectionSucceeded
// Created: PHC 2011-05-20
// -----------------------------------------------------------------------------
void CSVFileLogger::ConnectionSucceeded( const std::string& endpoint )
{
    Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";warning;;;"
         + "Connection succeeded at '" + endpoint + "'" );
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger::AuthenticationSucceeded
// Created: PHC 2011-05-20
// -----------------------------------------------------------------------------
void CSVFileLogger::AuthenticationSucceeded( const std::string& profile )
{
    Write( bpt::to_simple_string( bpt::second_clock::local_time() ) + ";warning;;;"
         + "Authentication succeeded with profile '" + profile + "'" );
}

// -----------------------------------------------------------------------------
// Name: CSVFileLogger::WriteHeader
// Created: PHC 2011-05-25
// -----------------------------------------------------------------------------
void CSVFileLogger::WriteHeader()
{
    logger_->Write( "date;type;unitID;orderID;details" );
}
