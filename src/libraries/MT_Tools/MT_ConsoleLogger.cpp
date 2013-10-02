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

void MT_ConsoleLogger::WriteString( const std::string& s )
{
    boost::mutex::scoped_lock locker( mutex_ );
    std::cout << s << std::endl;
}
