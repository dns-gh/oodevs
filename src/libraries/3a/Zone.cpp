// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma warning( disable: 4996 ) // Function call with parameters that may be unsafe
#include "Zone.h"
#include "Position.h"
#include "Circle.h"
#include "Polygon.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <vector>

namespace
{
    class WorldZone : public Zone_ABC
    {
        virtual bool Contains( const Position& ) const
        {
            return true;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Zone constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Zone::Zone()
    : zone_( new WorldZone() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Zone constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Zone::Zone( double )
    : zone_( new WorldZone() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Zone destructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Zone::~Zone()
{
    // NOTHING
}

namespace
{
    Zone_ABC* CreateZone( const std::string& type, const std::string& coordinates )
    {
        std::vector< std::string > split;
        boost::algorithm::split( split, coordinates, boost::algorithm::is_any_of( "," ) );
        std::vector< Position > positions;
        std::transform( split.begin(), split.end(), std::back_inserter( positions ), &boost::lexical_cast< Position, std::string > );

        if( type == "circle" )
            return new Circle( positions );
        if( type == "polygon" )
            return new Polygon( positions );
        throw std::runtime_error( "Unknown zone type " + type );
    }
}

// -----------------------------------------------------------------------------
// Name: Zone::Read
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
void Zone::Read( const std::string& content )
{
    if( ! content.empty() )
    {
        std::string::size_type first = content.find_first_of( "(" );
        std::string::size_type last  = content.find_last_of ( ")" );
        if( first == std::string::npos || last == std::string::npos || last <= first )
            throw std::runtime_error( "Invalid zone : " + content );
        const std::string type        = content.substr( 0, first );
        const std::string coordinates = content.substr( first+1, last - first - 1 );
        zone_.reset( CreateZone( type, coordinates ) );
    }
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
std::istream& operator>>( std::istream& is, Zone& zone )
{
    zone.Read( std::string( std::istreambuf_iterator< char >( is ), std::istreambuf_iterator< char >() ) );
    return is;
}

// -----------------------------------------------------------------------------
// Name: Zone::operator double
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Zone::operator double() const
{
    return 0.f;
}
