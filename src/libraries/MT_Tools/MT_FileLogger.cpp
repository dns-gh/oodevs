// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FileLogger.h"
#include <tools/StdFileWrapper.h>

#pragma warning( disable : 4355 )

MT_FileLogger::MT_FileLogger( const tools::Path& filename, std::size_t files, std::size_t size, int levels, bool truncate, E_Type type, bool sizeInBytes )
    : MT_Logger_ABC( levels, type )
    , log_( filename, files, size, truncate, sizeInBytes )
{
    // NOTHING
}

MT_FileLogger::~MT_FileLogger()
{
    // NOTHING
}

void MT_FileLogger::WriteString( const std::string& s )
{
    boost::mutex::scoped_lock locker( mutex_ );
    log_.Write( s );
}
