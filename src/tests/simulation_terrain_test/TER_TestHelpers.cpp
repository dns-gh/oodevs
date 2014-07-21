// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_Polygon.h"
#include <tools/Exception.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/regex.hpp>

namespace
{

std::string GetWKTType( const std::string& wkt )
{
    static const boost::regex reType( "^\\s*(\\S+)\\s*\\(", boost::regex::icase );
    boost::smatch result;
    if( !boost::regex_search( wkt, result, reType, boost::match_perl ) )
        throw MASA_EXCEPTION( "could not parse wkt type: " + wkt );
    return boost::algorithm::to_lower_copy( std::string( result[1] ) );
}

// Circle is not defined in WKT (only CircularString), but who cares:
//
//   CIRCLE( p1x p1y, p2x p2y )
//
// where the two points define a diameter.
boost::shared_ptr< TER_Localisation > CircleFromWKT( const std::string& wkt )
{
    static const boost::regex reCircle(
            "\\s*circle\\s*\\(\\s*(\\S+)\\s+(\\S+)\\s*,\\s*(\\S+)\\s+(\\S+)\\s*\\)",
            boost::regex::icase );
    boost::smatch result;
    if( !boost::regex_match( wkt, result, reCircle, boost::match_perl ) )
        throw MASA_EXCEPTION( "could not parse circle wkt: " + wkt );
    const MT_Vector2D p1( boost::lexical_cast< double >( result[1] ),
                          boost::lexical_cast< double >( result[2] ));
    const MT_Vector2D p2( boost::lexical_cast< double >( result[3] ),
                          boost::lexical_cast< double >( result[4] ));
    const MT_Vector2D center( ( p1.rX_ + p2.rY_ ) / 2, ( p1.rY_ + p2.rY_ ) / 2 );
    const double radius = p1.Distance( p2 ) / 2;
    return boost::make_shared< TER_Localisation >( center, radius );
}

} // namespace

TER_Polygon FromWKT( std::string wkt, bool convexHull )
{
    // It is tempting to use GEOS to parse the WKT but it would normalize
    // input geometries in the process, which would render the tests useless.
    static const boost::regex rePolygon( "\\s*polygon\\s*\\((.*)\\)\\s*",
           boost::regex::icase );
    static const boost::regex rePoint( "\\s*(\\S+)\\s+(\\S+)\\s*" );

    boost::smatch result;
    if( !boost::regex_match( wkt, result, rePolygon, boost::match_perl ) )
        throw MASA_EXCEPTION( "could not parse wkt: " + wkt );
    const std::string def = result[1].str();

    T_PointVector points;
    if( !def.empty() )
    {
        std::vector< std::string > coords;
        boost::split( coords, def, boost::is_any_of( "," ) );
        for( auto it = coords.begin(); it != coords.end(); ++it )
        {
            if( !boost::regex_match( *it, result, rePoint, boost::match_perl ) )
                throw MASA_EXCEPTION( "could not parse wkt point: " + *it );
            const auto x = boost::lexical_cast< double >( result[1] );
            const auto y = boost::lexical_cast< double >( result[2] );
            points.push_back( MT_Vector2D( x, y ) );
        }
    }
    TER_Polygon polygon;
    polygon.Reset( points, convexHull );
    return polygon;
}

boost::shared_ptr< TER_Localisation > LocalisationFromWKT( const std::string& wkt )
{
    const auto type = GetWKTType( wkt );
    if( type == "polygon" )
    {
        auto polygon = FromWKT( wkt, false );
        return boost::make_shared< TER_Localisation >( polygon );
    }
    else if( type == "circle" )
    {
        return CircleFromWKT( wkt );
    }
    throw MASA_EXCEPTION( "unknown WKT type: " + type );
}
