// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_Polygon.h"
#include <tools/BoostTest.h>
#include <tools/Exception.h>
#include <tools/Helpers.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <sstream>

namespace
{

// Parses expressions like:
//
//   POLYGON (5 6, 6 7, 7 8)
//
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

std::string ToWKT( const TER_Polygon& p )
{
    std::stringstream out;
    out << "POLYGON (";
    const auto& points = p.GetBorderPoints();
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        if( it != points.begin() )
            out << ", ";
        out << it->rX_ << " " << it->rY_;
    }
    out << ")";
    return out.str();
}

}  // namespace

BOOST_AUTO_TEST_CASE( TER_Polygon_IsInside )
{
    const auto poly = FromWKT( "POLYGON (0 0, 5 0, 5 5)", false );

    BOOST_CHECK( poly.IsInside( MT_Vector2D( 2, 1 ) ));
    BOOST_CHECK( !poly.IsInside( MT_Vector2D( 6, 6 ) ));

    TER_Polygon empty;
    BOOST_CHECK( empty.IsInside( MT_Vector2D( 6, 6 ) ));
}

namespace
{

struct TestConstructor
{
    std::string wkt;
    bool convexHull;
    std::string expected;
};

} // namespace

BOOST_AUTO_TEST_CASE( TER_Polygon_Constructor )
{
    const TestConstructor tests[] =
    {
        // Empty polygons
        { "POLYGON ()", false, "POLYGON ()" },
        { "POLYGON ()", true, "POLYGON ()" },

        // Invalid polygons
        { "POLYGON (1 1)", false, "POLYGON ()" },
        { "POLYGON (1 1)", true, "POLYGON ()" },

        // Self-intersecting shapes (bow tie)
        { "POLYGON (0 0, 2 1, 2 0, 0 1, 0 0)", false, "POLYGON (1 0.5, 2 0, 2 1, 1 0.5)" },
        { "POLYGON (0 0, 2 1, 2 0, 0 1, 0 0)", true, "POLYGON (0 0, 2 0, 2 1, 0 1, 0 0)" },

        // Regular concave polygon (right looking rectangular pacman)
        { "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3)", false, "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3, 0 0)" },
        { "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3)", true, "POLYGON (0 0, 2 0, 2 3, 0 3, 0 0)" },
    };

    for( size_t i = 0; i != COUNT_OF( tests ); ++i )
    {
        const auto& t = tests[i];
        auto p = FromWKT( t.wkt, t.convexHull );
        const auto res = ToWKT( p );
        const auto err = tools::CheckOutput( t.expected, res );
        if( !err.empty() )
            BOOST_ERROR( err );
    }
}
