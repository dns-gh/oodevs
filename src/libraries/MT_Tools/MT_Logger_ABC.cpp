// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Logger_ABC.h"
#pragma warning( disable: 4996 )
#include <ctime>

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC constructor
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
MT_Logger_ABC::MT_Logger_ABC( unsigned int nLogLevels, unsigned int nLogLayers )
    : bPaused_     ( false )
//, strTimestamp_ ( "%a, %d %b %Y %H:%M:%S" ) // Sample : "Mon, 05 Jun 2000 11:38:52"
    , strTimestamp_( "%H:%M:%S" ) // Sample : "11:38:52"
    , nLogLevels_  ( nLogLevels )
    , nLogLayers_  ( nLogLayers )
{
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC destructor
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
MT_Logger_ABC::~MT_Logger_ABC()
{
}

//=============================================================================
// MAIN METHOD
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Log
/** @param  level EXPLANATION
    @param  strMessage EXPLANATION
    @param  nTime EXPLANATION
    @return PUT YOUR RETURN VALUE AND ITS EXPLANATION

  PUT YOUR COMMENTS HERE
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
void MT_Logger_ABC::Log( unsigned int nLayer, const char* strLayerName, E_LogLevel nLevel, const char* strMessage, const char* strContext, int nCode )
{
    if( bPaused_ == true )
        return;

    if( IsLogLevelSet( nLevel ) == false )
        return;

    if( IsLogLayerSet( nLayer ) == false )
        return;
    
    LogString( strLayerName, nLevel, strMessage, strContext, nCode );
}


//=============================================================================
// TOOLS
//============================================================================

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::GetTimestampAsString
/** 
    @return Return a formated timestamp
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
const char* MT_Logger_ABC::GetTimestampAsString()
{
    static char buffer[256];

    time_t nTime = time( NULL );
    
    strftime( buffer, 256, strTimestamp_.c_str(), localtime( &nTime ) );
    return buffer;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::GetLogLevelAsString
/** 
    @return The log level name as a string
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
const char* MT_Logger_ABC::GetLogLevelAsString( E_LogLevel nLevel )
{
    switch( nLevel )
    {
        case eLogLevel_FatalError:
            return "Fatal";
        case eLogLevel_Error:
            return "Error";
        case eLogLevel_Warning:
            return "Warning";
        case eLogLevel_Message:
            return "Message";
        case eLogLevel_Info:
            return "Info";
        case eLogLevel_Verbose:
            return "Verbose";
        case eLogLevel_Debug:
            return "Debug";
        case eLogLevel_None:
            return "";
        default:
            return "Unknown log level";
    }
}
