// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FileLogger.h"
#include "tools/FileLog.h"
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
    struct FileLog : tools::FileLog
    {
        FileLog( const tools::Path& filename, bool sizeInBytes )
            : tools::FileLog( filename )
            , s_          ( filename, std::ios::out | std::ios::app )
            , sizeInBytes_( sizeInBytes )
        {}
        virtual std::size_t Write( const std::string& line )
        {
            s_ << line << std::flush;
            return sizeInBytes_ ? line.size() : 1;
        }
        virtual void Close()
        {
            s_.close();
        }
        tools::Ofstream s_;
        bool sizeInBytes_;
    };
}

std::auto_ptr< tools::Log_ABC > MT_FileLogger::CreateLog( const tools::Path& filename, std::streamoff& size )
{
    if( truncate_ )
        filename.Remove();
    else
        size = ComputeSize( filename );
    return std::auto_ptr< tools::Log_ABC >( new FileLog( filename, sizeInBytes_ ) );
}

std::streamoff MT_FileLogger::ComputeSize( const tools::Path& filename ) const
{
    if( sizeInBytes_ )
        return filename.FileSize();
    tools::Ifstream file( filename );
    return std::count( std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >(), '\n');
}
