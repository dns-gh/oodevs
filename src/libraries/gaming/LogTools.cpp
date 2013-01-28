// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogTools.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include <boost/lexical_cast.hpp>

namespace log_tools
{

// -----------------------------------------------------------------------------
// Name: LogAcknowledge
// Created: AGE 2006-07-06
// -----------------------------------------------------------------------------
void LogAcknowledge( kernel::Logger_ABC& logger, const std::string& messageName )
{
    logger.Info( messageName + " acknowledged ok" );
}

// -----------------------------------------------------------------------------
// Name: LogAcknowledge
// Created: AGE 2006-07-06
// -----------------------------------------------------------------------------
void LogAcknowledge( kernel::Logger_ABC& logger, const char* messageName )
{
    logger.Info( std::string( messageName ) + " acknowledged ok" );
}

// -----------------------------------------------------------------------------
// Name: CheckAcknowledge
// Created: AGE 2006-07-06
// -----------------------------------------------------------------------------
bool CheckAcknowledge( kernel::Logger_ABC& logger, int errorCode, const std::string& errorMessage, const std::string& messageName )
{
    if( errorCode )
    {
        logger.Warning( messageName + " acknowledge error: " + errorMessage );
        return false;
    }
    else
    {
        logger.Info( messageName + " acknowledged ok" );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: CheckAcknowledge
// Created: NLD 2011-05-25
// -----------------------------------------------------------------------------
bool CheckAcknowledge( kernel::Logger_ABC& logger, const kernel::Entity_ABC& entity, int errorCode, const std::string& errorMessage, const std::string& messageName )
{
    const std::string str = messageName + " [" + entity.GetName().toStdString() + " " + boost::lexical_cast< std::string >( entity.GetId() ) + "] ";
    if( errorCode )
    {
        logger.Warning( str + " acknowledge error: " + errorMessage );
        return false;
    }
    else
    {
        logger.Info( str + " acknowledged ok" );
        return true;
    }
}

}
