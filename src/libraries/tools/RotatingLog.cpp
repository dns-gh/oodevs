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
#include "Log_ABC.h"
#include <tools/StdFileWrapper.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

using namespace tools;

RotatingLog::RotatingLog( tools::Log_ABC& log, const tools::Path& filename, std::size_t files, std::size_t size, bool truncate )
    : log_     ( log )
    , filename_( filename.Normalize() )
    , files_   ( files )
    , size_    ( size )
    , count_   ( log_.ComputeSize( filename_ ) )
{
    Populate();
    if( truncate && filename_.Exists() )
        Rotate();
    Prune();
}

RotatingLog::~RotatingLog()
{
    // NOTHING
}

void RotatingLog::Populate()
{
    const tools::Path stem = filename_.Parent() / filename_.BaseName();
    const boost::regex regex( "\\Q" + stem.ToUTF8() + "\\E\\.\\d{8}T\\d{6}\\.log(\\.\\d+)*" );
    filename_.Parent().ListElements(
        [&]( const Path& path ) -> bool
        {
            if( boost::regex_match( path.ToUTF8(), regex ) )
                history_.push_back( path );
            return false;
        },
        false );
    std::sort( history_.begin(), history_.end() );
}

void RotatingLog::DoWrite( const std::string& line )
{
    try
    {
        if( size_ != 0 && count_ >= size_ && stream_ )
        {
            Rotate();
            Prune();
        }
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

void RotatingLog::Log( const std::string& line )
{
    if( ! stream_ )
        stream_.reset( new Ofstream( filename_, count_ > 0 ? std::ios::app : std::ios::trunc ) );
    count_ += log_.Write( *stream_, line );
}

void RotatingLog::Prune()
{
    while( history_.size() >= files_ && ! history_.empty() )
    {
        history_.front().Remove();
        history_.pop_front();
    }
}

void RotatingLog::Rotate()
{
    const tools::Path filename = filename_.Parent() / filename_.BaseName() + "." +
        boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ).c_str() + filename_.Extension();
    tools::Path to = filename;
    for( int i = 1; to.Exists(); ++i )
        to = filename + "." + boost::lexical_cast< std::string >( i ).c_str();
    stream_.reset();
    count_ = 0;
    filename_.Rename( to );
    history_.push_back( to );
}
