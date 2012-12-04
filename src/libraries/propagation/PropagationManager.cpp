// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PropagationManager.h"
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: PropagationManager constructor
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
PropagationManager::PropagationManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationManager destructor
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
PropagationManager::~PropagationManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::Initialize
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
void PropagationManager::Initialize( const std::string& config, const std::string& time )
{
    bfs::path path( config );
    bfs::path parent( path.parent_path() );
    xml::xifstream xis( path.string() );
    boost::posix_time::ptime startTime;
    boost::posix_time::time_duration delta;
    if( time != "" )
       startTime = boost::posix_time::from_iso_string( time );
    xis >> xml::start( "config" )
            >> xml::content( "projection", projection_ )
                >> xml::start( "files" )
                    >> xml::list( "file", *this, &PropagationManager::ReadFile, parent, startTime, delta )
                >> xml::end
        >> xml::end;

    projection_ = bfs::path( parent / projection_ ).string();
}

namespace
{
    boost::posix_time::ptime Convert( const std::string& date )
    {
        std::string time = date;
        time.erase( std::remove( time.begin(), time.end(), '-' ), time.end() );
        time.erase( std::remove( time.begin(), time.end(), ':' ), time.end() );
        return boost::posix_time::from_iso_string( time );
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::ReadFile
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
void PropagationManager::ReadFile( xml::xistream& xis, const boost::filesystem::path& path,
                                   const boost::posix_time::ptime& startTime, boost::posix_time::time_duration & delta )
{
    boost::posix_time::ptime time = Convert( xis.attribute< std::string >( "time" ) );
    if( !startTime.is_not_a_date_time() && schedule_.empty() && startTime > time )
        delta = startTime - time;
    if( !delta.is_not_a_date_time() )
        time += delta;
    schedule_[ time ].push_back( bfs::path( path / bfs::path( xis.value< std::string >() ).filename() ).string() );
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetProjectionFile
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
const std::string& PropagationManager::GetProjectionFile() const
{
    return projection_;
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetFiles
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
PropagationManager::T_Files PropagationManager::GetFiles( const std::string& time )
{
    const boost::posix_time::ptime ptime( boost::posix_time::from_iso_string( time ) );
    T_Files files;
    for( CIT_Schedule it = schedule_.begin(); it != schedule_.end(); ++it )
        if( ptime >= it->first )
            files = it->second;
    return files;
}
