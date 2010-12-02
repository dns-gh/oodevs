// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_ConsoleLogger.h"
#include <cstdio>

//-----------------------------------------------------------------------------
// Name: MT_ConsoleLogger constructor
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
MT_ConsoleLogger::MT_ConsoleLogger( unsigned int nLogLevels, unsigned int nLayers )
    : MT_Logger_ABC( nLogLevels, nLayers )
    , bDumpTimeStamps_( true  )
{
    // NOTHING 
}

//-----------------------------------------------------------------------------
// Name: MT_ConsoleLogger::~MT_ConsoleLogger
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_ConsoleLogger::~MT_ConsoleLogger()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_ConsoleLogger::GetColorFromLogLevel
/** 
    @return The ANSI color associated to the given log level
*/
// Created:  NLD 00-10-23 
//-----------------------------------------------------------------------------
const char* MT_ConsoleLogger::GetColorFromLogLevel( unsigned int nLogLevel )
{
    switch( nLogLevel )
    {
        case eLogLevel_Error:
            return "\033[31;5m";
        case eLogLevel_Warning:
            return "\033[33;5m";
        default:
            return "";
    }
}

//-----------------------------------------------------------------------------
// Name: MT_ConsoleLogger::EnableTimeStamps
/** 
*/
// Created: CBX 2003-02-13
//-----------------------------------------------------------------------------
void MT_ConsoleLogger::EnableTimeStamps( bool bEnabled )
{
    bDumpTimeStamps_ = bEnabled;
}

//-----------------------------------------------------------------------------
// Name: MT_ConsoleLogger::LogString
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
void MT_ConsoleLogger::LogString( const char* strLayerName, E_LogLevel level, const char* strMessage, const char* strContext, int nCode )
{
    boost::mutex::scoped_lock locker( mutex_ );

#ifdef MT_COLOR_CONSOLE
    printf( "%s", GetColorFromLogLevel( level ) );
#endif

    // Time
    if( bDumpTimeStamps_ )
        printf( "[%s]", GetTimestampAsString() );

    // Sub level name
    if( strLayerName != NULL )
        printf( " %s -", strLayerName );

    // Log level name
    if( level != eLogLevel_None )
        printf( " %s -", GetLogLevelAsString(level) );

    // Message
    if( strMessage )
        printf( " %s", strMessage );
    else 
        printf( " " );

    // Code
    if( nCode != -1 )
        printf( " (%d)", nCode );

    // Context
    if( strContext != NULL )
        printf( " [Context: %s]", strContext );

#ifdef MT_COLOR_CONSOLE
    printf("\033[0m\n");
#else
    printf("\n");
#endif
}
