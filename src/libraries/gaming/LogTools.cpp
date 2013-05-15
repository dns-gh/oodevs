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
#include "clients_kernel/Tools.h"

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
bool CheckAcknowledge( kernel::Logger_ABC& logger, int errorCode, const std::string& errorMessage, const std::string& messageName, bool display )
{
    if( display )
    {
        if( errorCode )
            logger.Warning( messageName + " acknowledge error: " + errorMessage );
        else
            logger.Info( messageName + " acknowledged ok" );
    }
    return !errorCode;
}

// -----------------------------------------------------------------------------
// Name: CheckAcknowledge
// Created: NLD 2011-05-25
// -----------------------------------------------------------------------------
bool CheckAcknowledge( kernel::Logger_ABC& logger, const kernel::Entity_ABC& entity, int errorCode, const std::string& errorMessage,
                      const std::string& messageName, bool display )
{
    if( display )
    {
        const std::string str = messageName + " [" + entity.GetName().toStdString() + " " + boost::lexical_cast< std::string >( entity.GetId() ) + "] ";
        if( errorCode )
            logger.Warning( str + " acknowledge error: " + errorMessage );
        else
            logger.Info( str + " acknowledged ok" );
    }
    return !errorCode;
}

// -----------------------------------------------------------------------------
// Name: Convert
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
std::string Convert( sword::OrderAck_ErrorCode code )
{
    if( code == sword::OrderAck_ErrorCode_error_invalid_unit )
        return tools::translate( "LogTools", "Invalid unit" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_limit )
        return tools::translate( "LogTools", "Invalid limit" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_lima )
        return tools::translate( "LogTools", "Invalid lima" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_mission )
        return tools::translate( "LogTools", "Invalid mission" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_parameter )
        return tools::translate( "LogTools", "Invalid parameter" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_frag_order )
        return tools::translate( "LogTools", "Invalid frag order" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_order_mission )
        return tools::translate( "LogTools", "Invalid order mission" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_unit_surrendered )
        return tools::translate( "LogTools", "Unit surrendered" ).toStdString();
    if( code == sword::OrderAck_ErrorCode_error_invalid_lima_function )
        return tools::translate( "LogTools", "Invalid lima function" ).toStdString();
    return "";
}

}
