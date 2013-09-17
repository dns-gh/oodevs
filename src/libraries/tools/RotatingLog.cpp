// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "RotatingLog.h"
#include "LogFactory_ABC.h"
#include "Log_ABC.h"
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/regex.hpp>

using namespace tools;

RotatingLog::RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, std::size_t files, std::size_t size, bool truncate )
    : factory_ ( factory )
    , filename_( filename )
    , files_   ( files )
    , size_    ( size )
    , count_   ( 0 )
{
    Populate();
    if( truncate && ! logs_.empty() )
        Rotate();
}

RotatingLog::~RotatingLog()
{
    // NOTHING
}

void RotatingLog::Populate() // $$$$ MCO 2013-09-13: double check RotatingLog usage in regards to "bool truncate"
{
    const tools::Path parent = filename_.Parent();
    parent.ListElements(
        [&]( const Path& path ) -> bool
        {
            if( boost::regex_match( path.ToUTF8(), boost::regex( ".+\\.\\d{8}T\\d{6}\\.log" ) ) )
                logs_.push_back( factory_.CreateLog( path, count_ ) );
            return false;
        } );
    if( ! logs_.empty() )
        logs_.push_back( factory_.CreateLog( filename_, count_ ) );
}

void RotatingLog::DoWrite( const std::string& line )
{
    try
    {
        Rotate();
    }
    catch( std::exception& e )
    {
        Log( "Failed to rotate log file : " + std::string( e.what() ) + '\n' );
    }
    catch( ... )
    {
        Log( "Failed to rotate log file for an unknown reason\n" );
    }
    Log( line );
}

namespace
{
    tools::Path Rename( const tools::Path& filename )
    {
        static const std::locale loc( std::wcout.getloc(), new boost::posix_time::time_facet( "%Y%m%dT%H%M%S" ) );
        std::stringstream s;
        s.imbue( loc );
        s << "." << boost::posix_time::second_clock::local_time();
        return filename.Parent() / filename.BaseName() + tools::Path::FromUTF8( s.str() ) + filename.Extension();
    }
}

void RotatingLog::Rotate()
{
    if( size_ == 0 || count_ < size_ )
        return;
    logs_.back().Rename( Rename( filename_ ) );
    CreateLog();
    while( logs_.size() > files_ )
    {
        logs_.front().Delete();
        logs_.pop_front();
    }
}

void RotatingLog::Log( const std::string& line )
{
    if( logs_.empty() )
        CreateLog();
    count_ += logs_.back().Write( line );
}

void RotatingLog::CreateLog()
{
    count_ = 0;
    logs_.push_back( factory_.CreateLog( filename_, count_ ) );
}
