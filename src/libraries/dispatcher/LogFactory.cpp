// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogFactory.h"
#include "Log.h"

using namespace dispatcher;

LogFactory::LogFactory( bool sizeInBytes )
    : sizeInBytes_( sizeInBytes )
{
    // NOTHING
}

std::auto_ptr< tools::Log_ABC > LogFactory::CreateLog( const tools::Path& filename, std::streamoff& /*size*/ )
{
    return std::auto_ptr< tools::Log_ABC >( new Log( filename, sizeInBytes_ ) );
}
