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
#include <tools/StdFileWrapper.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

using namespace tools;

class RotatingLog::Stream : boost::noncopyable
{
public:
    Stream( const tools::Path& filename )
        : filename_( filename )
        , stream_  ( filename, std::ios::app )
    {}
    void Rotate()
    {
        const tools::Path filename = filename_.Parent() / filename_.BaseName() + "." +
            boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() ).c_str() + filename_.Extension();
        tools::Path to = filename;
        for( int i = 1; to.Exists(); ++i )
            to = filename + "." + boost::lexical_cast< std::string >( ++i ).c_str();
        stream_.close();
        filename_.Rename( to );
        filename_ = to;
    }
    void Remove()
    {
        stream_.close();
        filename_.Remove();
    }
    std::size_t Write( tools::LogFactory_ABC& factory, const std::string& line )
    {
        return factory.Write( stream_, line );
    }

private:
    tools::Path filename_;
    tools::Ofstream stream_;
};

RotatingLog::RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, std::size_t files, std::size_t size, bool truncate )
    : factory_ ( factory )
    , filename_( filename )
    , files_   ( files )
    , size_    ( size )
    , count_   ( 0 )
{
    Populate();
    if( truncate )
    {
        if( files_ <= 1 )
            filename_.Remove();
        else
            Rotate();
    }
}

RotatingLog::~RotatingLog()
{
    // NOTHING
}

void RotatingLog::Populate()
{
    filename_.Parent().ListElements(
        [&]( const Path& path ) -> bool
        {
            if( boost::regex_match( path.ToUTF8(), boost::regex( ".+\\.\\d{8}T\\d{6}\\.log.*" ) ) )
                logs_.push_back( new Stream( path ) );
            return false;
        },
        false );
    if( ! logs_.empty() )
        CreateLog();
}

void RotatingLog::DoWrite( const std::string& line )
{
    try
    {
        if( size_ != 0 && count_ >= size_ )
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

void RotatingLog::Rotate()
{
    if( logs_.empty() )
        return;
    logs_.back().Rotate();
    CreateLog();
    while( logs_.size() > files_ )
    {
        logs_.front().Remove();
        logs_.pop_front();
    }
}

void RotatingLog::Log( const std::string& line )
{
    if( logs_.empty() )
        CreateLog();
    count_ += logs_.back().Write( factory_, line );
}

void RotatingLog::CreateLog()
{
    count_ = factory_.ComputeSize( filename_ );
    logs_.push_back( new Stream( filename_ ) );
}
