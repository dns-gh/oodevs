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
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Entity_ABC.h"
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
// Name: Logger::MissionCreated
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::MissionCreated( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const
{
    os_ << "===============================================================================" << std::endl
        << boost::posix_time::second_clock::local_time() << std::endl
        << "Agent ID: " << target.GetId() << "\t" << "Agent Name: " << target.GetName() << std::endl
        << "mission ID: " << mission.GetId()<< "\t" << "Mission Name: " << mission.GetName() << std::endl
        << "===============================================================================" << std::endl
        << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::ConnectionSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::ConnectionSucceeded( const std::string& endpoint ) const
{
    os_ << "Connection succeeded at " << endpoint << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::AuthenticationSucceeded
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Logger::AuthenticationSucceeded( const std::string& profile ) const
{
    os_ << "Authentication succeeded with profile '" << profile << "'" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Logger::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::MissionType& mission, const kernel::OrderParameter& parameter ) const
{
    os_ << "[" << target.GetName() << "][" << mission.GetName() << "][" << parameter.GetName() << "]: failed to create parameter" << std::endl;
}
