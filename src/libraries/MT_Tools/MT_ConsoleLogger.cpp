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
MT_ConsoleLogger::MT_ConsoleLogger( int nLogLevels )
    : MT_Logger_ABC( nLogLevels )
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
// Name: MT_ConsoleLogger::LogString
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_ConsoleLogger::LogString( E_LogLevel level, const char* strMessage, const char* strContext, int nCode )
{
    boost::mutex::scoped_lock locker( mutex_ );

    // Time
    printf( "[%s]", GetTimestampAsString() );

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

    printf("\n");
}
