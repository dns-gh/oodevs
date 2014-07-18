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
    auto polygon = FromWKT( wkt, false );
    return boost::make_shared< TER_Localisation >( polygon );
}
