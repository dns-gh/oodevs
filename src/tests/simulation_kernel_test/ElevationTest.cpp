// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Meteo/RawVisionData/Elevation.h"
#include "MT_Tools/MT_Vector2D.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace
{

// TODO: move it in tools, reuse in terrain
std::string PrefixAndEscapeLines( const std::string& prefix, const std::string& data )
{
    std::vector< std::string > lines;
    boost::split( lines, data, boost::is_any_of( "\n" ) );
    std::stringstream prefixed;
    for( auto it = lines.cbegin(); it != lines.cend(); ++it )
    {
        if( it->empty() )
            continue;
        prefixed << prefix << "\"" << *it << "\\n\"\n";
    }
    return prefixed.str();
}

// TODO: move it in tools, reuse in terrain
std::string CheckOutput( const std::string& out, const std::string& expected )
{
    if( out == expected )
        return "";
    const char* prefix = "            ";
    std::stringstream err;
    err << "Unexpected output:\n";
    err << PrefixAndEscapeLines( prefix, out );
    err << "!=\n";
    err << PrefixAndEscapeLines( prefix, expected );
    return err.str();
}

std::string CheckSplit( int32_t cellSize, double x1, double y1, double x2, double y2,
        const std::function< bool( MT_Vector2D, MT_Vector2D)>& f, std::string expected )
{
    std::stringstream output;
    auto callback = [&]( MT_Vector2D p1, MT_Vector2D p2 ) -> bool
    {
        output << "(" << int( p1.GetX() ) << ", " << int( p1.GetY() ) << ")";
        output << " -> ";
        output << "(" << int( p2.GetX() ) << ", " << int( p2.GetY() ) << ")";
        output << "\n";
        return f( p1, p2 );
    };
    const auto from = MT_Vector2D( x1, y1 );
    const auto to = MT_Vector2D( x2, y2 );
    const bool res = SplitOnMajorGridLines( cellSize, from, to, callback );
    if( res )
        output << "STOP\n";
    return CheckOutput( output.str(), expected );
}

bool DummyCallback( MT_Vector2D, MT_Vector2D )
{
    return false;
}

#define CHECK_SPLIT( x1, y1, x2, y2, expected )                                \
    {                                                                                    \
        auto err = CheckSplit( 100, x1, y1, x2, y2, DummyCallback, expected ); \
        if( !err.empty() )                                                               \
            BOOST_ERROR( err );                                                          \
    }

}  // namespace

BOOST_AUTO_TEST_CASE( path_split_on_major_grid_lines )
{
    // nul segment
    CHECK_SPLIT( 20, 25, 20, 25, 
        ""
    )

    // same cell
    CHECK_SPLIT( 20, 25, 10, 15, 
        "(20, 25) -> (10, 15)\n"
    )

    // adjacent horizontal cells
    CHECK_SPLIT( 120, 25, 80, 40, 
        "(120, 25) -> (100, 32)\n"
        "(100, 32) -> (80, 40)\n"
    )

    // adjacent vertical cells
    CHECK_SPLIT( 120, 25, 120, 180, 
        "(120, 25) -> (120, 100)\n"
        "(120, 100) -> (120, 180)\n"
    )

    // diagonal cells
    CHECK_SPLIT( 125, 125, 20, 20, 
        "(125, 125) -> (100, 100)\n"
        "(100, 100) -> (20, 20)\n"
    )

    // horizontal multisplit on grid line
    CHECK_SPLIT( -112, 0, 234, 0, 
        "(-112, 0) -> (-100, 0)\n"
        "(-100, 0) -> (0, 0)\n"
        "(0, 0) -> (100, 0)\n"
        "(100, 0) -> (200, 0)\n"
        "(200, 0) -> (234, 0)\n"
    )

    // vertical multisplit on grid line
    CHECK_SPLIT( 100, -112, 100, 234,
        "(100, -112) -> (100, -100)\n"
        "(100, -100) -> (100, 0)\n"
        "(100, 0) -> (100, 100)\n"
        "(100, 100) -> (100, 200)\n"
        "(100, 200) -> (100, 234)\n"
    )

    // Random diagonal with x major axis and end point on the grid
    CHECK_SPLIT( 10, 550, 700, 230,
        "(10, 550) -> (100, 508)\n"
        "(100, 508) -> (200, 462)\n"
        "(200, 462) -> (300, 416)\n"
        "(300, 416) -> (400, 369)\n"
        "(400, 369) -> (500, 323)\n"
        "(500, 323) -> (600, 276)\n"
        "(600, 276) -> (700, 230)\n"
    )

    // Random diagonal with y major axis and start point on the grid
    CHECK_SPLIT( 330, 600, 40, 90,
        "(330, 600) -> (273, 500)\n"
        "(273, 500) -> (216, 400)\n"
        "(216, 400) -> (159, 300)\n"
        "(159, 300) -> (103, 200)\n"
        "(103, 200) -> (46, 100)\n"
        "(46, 100) -> (40, 90)\n"
    )
}

BOOST_AUTO_TEST_CASE( path_split_on_major_grid_lines_with_stop )
{
    // Random diagonal with y major axis and start point on the grid
    auto callback = [&]( MT_Vector2D p1, MT_Vector2D ) -> bool
    {
        return p1.GetX() < 170.;
    };
    const auto err = CheckSplit( 100, 330, 600, 40, 90, callback,
        "(330, 600) -> (273, 500)\n"
        "(273, 500) -> (216, 400)\n"
        "(216, 400) -> (159, 300)\n"
        "(159, 300) -> (103, 200)\n"
        "STOP\n"
    );
    if( !err.empty() )
        BOOST_ERROR( err );
}


/*
BOOST_AUTO_TEST_CASE( path_split_on_major_grid_lines_benchmark )
{
    auto from = MT_Vector2D( 0, 0 );
    auto to = MT_Vector2D( 800*1000, 600*1000 );
    to = MT_Vector2D( 0.7*1000, 1.2*1000 );
    to = MT_Vector2D( 0.07*1000, 0.12*1000 );
    auto start = std::clock();
    const int loop = 1000000;
    for( int i = 0; i != loop; ++i )
    {
        SplitOnMajorGridLines( 100, from, to, DummyCallback );
    }
    auto end = std::clock();
    std::cerr << double(loop)/(1000.0*(end - start) / CLOCKS_PER_SEC) 
        << " call/ms" << std::endl;
}
*/
