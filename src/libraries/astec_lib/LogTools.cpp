// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogTools.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "Types.h"

namespace log_tools
{

// -----------------------------------------------------------------------------
// Name: CheckAcknowledge
// Created: AGE 2006-07-06
// -----------------------------------------------------------------------------
bool CheckAcknowledge( int errorCode, const char* logMessage )
{
    if( errorCode )
    {
        MT_LOG_WARNING( logMessage << " acknowledge error", eReceived, 0 );
        return false;
    }
    else
    {
        MT_LOG_INFO( logMessage << " acknowledged ok", eReceived, 0 );
        return true;
    }
}

}