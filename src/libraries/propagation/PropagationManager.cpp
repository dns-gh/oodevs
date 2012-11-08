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
void PropagationManager::Initialize( const std::string& config )
{
    bfs::path path( config );
    bfs::path parent( path.parent_path() );
    xml::xifstream xis( path.string() );
    xis >> xml::start( "config" )
            >> xml::content( "projection", projection_ )
                >> xml::start( "files" )
                    >> xml::list( "file", *this, &PropagationManager::ReadFile, parent )
                >> xml::end
                >> xml::optional
                    >> xml::start( "colors" )
                        >> xml::list( "color", *this, &PropagationManager::ReadColor )
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
void PropagationManager::ReadFile( xml::xistream& xis, const boost::filesystem::path& path )
{
    schedule_[ Convert( xis.attribute< std::string >( "time" ) ) ]
        .push_back( bfs::path( path / bfs::path( xis.value< std::string >() ).filename() ).string() );
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::ReadColor
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
void PropagationManager::ReadColor( xml::xistream& xis )
{
    colors_[ xis.attribute< float >( "threshold" ) ] = xis.value< std::string >().c_str();
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetProjectionFile
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
const std::string& PropagationManager::GetProjectionFile() const
{
    return projection_;
}

namespace
{
    bool Compare( const std::vector< std::string >& lhs, const std::vector< std::string >& rhs )
    {
        if( lhs.size() != rhs.size() )
            return false;
        for( std::size_t i = 0; i < rhs.size(); ++i )
            if( lhs[ i ] != rhs[ i ] )
                return false;
        return true;
    }
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
    if( ! Compare( files, currentFiles_ ) )
    {
        currentFiles_ = files;
        return files;
    }
    return T_Files();
}

// -----------------------------------------------------------------------------
// Name: PropagationManager::GetColor
// Created: LGY 2012-11-07
// -----------------------------------------------------------------------------
std::string PropagationManager::GetColor( float value ) const
{
    if( colors_.empty() )
        return "#00FF00";
    CIT_Colors it = colors_.lower_bound( value );
    if( it == colors_.end() )
        return "#00FF00";
    return it->second;
}
