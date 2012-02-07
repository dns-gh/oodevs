// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FileLogger.h"
#include <cstdio>

#ifdef WIN32
#   define snprintf _snprintf
#endif

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::MT_FileLogger( const char* strFileName, int nLogLevels, bool bClearPreviousLog, E_Type type )
    : MT_Logger_ABC( nLogLevels, type )
{
    if( bClearPreviousLog )
        file_.open( strFileName, std::ios::out | std::ios::trunc );
    else
        file_.open( strFileName, std::ios::out | std::ios::app );
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::~MT_FileLogger()
{
    file_.close();
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::LogString
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_FileLogger::LogString( E_LogLevel nLogLevel, const char* strMessage, const char* strContext, int nCode )
{
    boost::mutex::scoped_lock locker( mutex_ );

    file_ << "[" << GetTimestampAsString() << "] ";
    file_ << "<" << GetTypeAsString() << "> ";

    // Level name
    file_ << "<" << GetLogLevelAsString( nLogLevel ) << "> ";

    // Msg
    if( strMessage )
        file_ << strMessage;

    // Code
    if( nCode != -1 )
        file_ << "(" << nCode << ") ";

    // Context
    if( strContext )
        file_ << "[Context: " << strContext << "]";

    file_ << std::endl;

    file_.flush();
}
