// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PropagationManager.h"
#include "ASCExtractor.h"
#include "DATExtractor.h"
#include "tools/FileWrapper.h"
#include "tools/XmlStreamOperators.h"
#include "tools/Exception.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

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
// Name: PropagationManager constructor
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
PropagationManager::PropagationManager( const tools::Path& config, const std::string& time )
{
    const tools::Path parent = config.Parent();
    tools::Xifstream xis( config );
    boost::posix_time::ptime startTime;
    boost::posix_time::time_duration delta;
    if( time != "" )
       startTime = boost::posix_time::from_iso_string( time );
    xis >> xml::start( "config" );
    if( xis.has_child( "projection" ) )
        xis >> xml::content( "projection", projection_ );
    else if( xis.has_child( "time-zone" ) )
        timeZone_ = xis.content< short >( "time-zone" );
    if( projection_.IsEmpty() && !timeZone_ )
        throw MASA_EXCEPTION( xis.context() + "Propagation file " + config.ToUTF8() + " not valid" );
    xis     >> xml::start( "files" )
                >> xml::list( "file", [&]( xml::xistream& x )
                {
                    boost::posix_time::ptime time = Convert( x.attribute< std::string >( "time" ) );
                    if( !startTime.is_not_a_date_time() && schedule_.empty() && startTime > time )
                        delta = startTime - time;
                    if( !delta.is_not_a_date_time() )
                        time += delta;
                    const auto filename = x.value< tools::Path >().FileName();
                    schedule_[ time ].push_back( ( parent / filename ).Normalize() );
                })
            >> xml::end
        >> xml::end;

    if( !projection_.IsEmpty() )
        projection_ = ( parent / projection_ ).Normalize();
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
// Name: PropagationManager::CreateExtractor
// Created: JSR 2014-04-14
// -----------------------------------------------------------------------------
boost::shared_ptr< Extractor_ABC > PropagationManager::CreateExtractor( const tools::Path& file ) const
{
    if( timeZone_ )
        return boost::make_shared< DATExtractor >( file, *timeZone_ );
    return boost::make_shared< ASCExtractor >( file, projection_ );
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetProjectionFile
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
const tools::Path& PropagationManager::GetProjectionFile() const
{
    return projection_;
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetFiles
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
tools::Path::T_Paths PropagationManager::GetFiles( const std::string& time ) const
{
    const boost::posix_time::ptime ptime( boost::posix_time::from_iso_string( time ) );
    tools::Path::T_Paths files;
    for( auto it = schedule_.begin(); it != schedule_.end(); ++it )
        if( ptime >= it->first )
            files = it->second;
    return files;
}
