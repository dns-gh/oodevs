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
#include "Types.h"
#include "clients_kernel/Logger_ABC.h"

namespace log_tools
{

// -----------------------------------------------------------------------------
// Name: CheckAcknowledge
// Created: AGE 2006-07-06
// -----------------------------------------------------------------------------
bool CheckAcknowledge( kernel::Logger_ABC& logger, int errorCode, const char* logMessage )
{
    if( errorCode )
    {
        logger.Warning() << logMessage << " acknowledge error";
        return false;
    }
    else
    {
        logger.Info() << logMessage << " acknowledged ok";
        return true;
    }
}

}