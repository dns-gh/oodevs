// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FileLogger.h"
#include "tools/Log_ABC.h"
#include <tools/FileWrapper.h>
#include <boost/lexical_cast.hpp>

#pragma warning( disable : 4355 )

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::MT_FileLogger( const tools::Path& filename, std::size_t files, std::size_t size, int levels, bool truncate, E_Type type, bool sizeInBytes )
    : MT_Logger_ABC( levels, type )
    , truncate_   ( truncate || files > 1 )
    , sizeInBytes_( sizeInBytes )
    , log_        ( *this, filename, files, size )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::~MT_FileLogger()
{
    // NOTHING
}

void MT_FileLogger::WriteString( const std::string& s )
{
    boost::mutex::scoped_lock locker( mutex_ );
    log_.Write( s );
}

namespace
{
    struct LogFile : tools::Log_ABC
    {
        LogFile( const tools::Path& filename, bool sizeInBytes )
            : s_          ( filename, std::ios::out | std::ios::app )
            , sizeInBytes_( sizeInBytes )
        {}
        virtual std::size_t Write( const std::string& s )
        {
            s_ << s;
            s_.flush();
            return sizeInBytes_ ? s.size() : 1;
        }
        tools::Ofstream s_;
        bool sizeInBytes_;
    };
}

std::auto_ptr< tools::Log_ABC > MT_FileLogger::CreateLog( const tools::Path& filename, std::size_t& size )
{
    if( truncate_ )
        filename.Remove();
    else
        size = ComputeSize( filename );
    return std::auto_ptr< tools::Log_ABC >( new LogFile( filename, sizeInBytes_ ) );
}

std::size_t MT_FileLogger::ComputeSize( const tools::Path& filename ) const
{
    if( sizeInBytes_ )
        return filename.FileSize();
    tools::Ifstream file( filename );
    return std::count( std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >(), '\n');
}
