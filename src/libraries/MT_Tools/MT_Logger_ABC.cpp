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
MT_Logger_ABC::MT_Logger_ABC( int nLogLevels )
    : bPaused_     ( false )
    , strTimestamp_( "%A %d - %H:%M:%S" ) // Sample : "Thursday 23 - 11:38:52 "
    , nLogLevels_  ( nLogLevels )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC destructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_Logger_ABC::~MT_Logger_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Log
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_Logger_ABC::Log( E_LogLevel nLevel, const char* strMessage, const char* strContext, int nCode )
{
    if( ! bPaused_ && IsLogLevelSet( nLevel ) )
        LogString( nLevel, strMessage, strContext, nCode );
}

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

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::SetTimestampFormat
/** @param  strFormat Format of the timestamp

  Set the timestamp format. See strftime() man for formats details.
*/
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_Logger_ABC::SetTimestampFormat( const char* strFormat )
{
    strTimestamp_ = strFormat;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::GetTimestampFormat
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
const char* MT_Logger_ABC::GetTimestampFormat() const
{
    return strTimestamp_.c_str();
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::IsLogLevelSet
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
bool MT_Logger_ABC::IsLogLevelSet( E_LogLevel nLevel ) const
{
    return (nLogLevels_ & nLevel) != 0;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Pause
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_Logger_ABC::Pause()
{
    bPaused_ = true;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Resume
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_Logger_ABC::Resume()
{
    bPaused_ = false;
}
