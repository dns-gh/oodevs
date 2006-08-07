// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "LogTools.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "astec_gaming/Types.h"

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
        MT_LOG_WARNING_MSG( logMessage << " acknowledge error" );
        return false;
    }
    else
    {
        MT_LOG_INFO_MSG( logMessage << " acknowledged ok" );
        return true;
    }
}

}