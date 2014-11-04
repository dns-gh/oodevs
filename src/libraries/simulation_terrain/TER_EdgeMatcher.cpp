// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_EdgeMatcher.h"
#include "TER_PathFinder_ABC.h"
#include "TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Vector2d.h"
#include "protocol/Simulation.h"
#include <boost/make_shared.hpp>

namespace
{

const float squareEpsilon = 100; // 10 meters

TerrainData ReadTerrainData( const sword::TerrainData& data )
{
    return TerrainData( static_cast< unsigned char >( data.area() ),
                        static_cast< unsigned char >( data.left() ),
                        static_cast< unsigned char >( data.right() ),
                        static_cast< unsigned short >( data.linear() ) );
}

TerrainPathPoint ReadPathPoint( const sword::PathPoint& point )
{
    MT_Vector2D position;
    TER_World::GetWorld().MosToSimMgrsCoord(
            point.coordinate().latitude(),
            point.coordinate().longitude(),
            position );
    return TerrainPathPoint( geometry::Point2f( static_cast< float >( position.GetX() ),
                                                static_cast< float >( position.GetY() ) ),
                             ReadTerrainData( point.current() ), ReadTerrainData( point.next() ) );
}

TER_EdgeMatcher::T_PathPoints ReadPathPoints( const sword::PathResult& result )
{
    TER_EdgeMatcher::T_PathPoints points;
    for( auto it = result.points().begin(); it != result.points().end(); ++it )
        points.push_back( std::make_pair( ReadPathPoint( *it ),
                          it->has_waypoint() ? it->waypoint() : -1 ) );
    return points;
}

}  // namespace

TER_EdgeMatcher::TER_EdgeMatcher( TER_Pathfinder_ABC& pathfinder,
        const sword::Pathfind& pathfind )
    : pathfinder_( pathfinder )
    , pathfind_( pathfind )
{
    // NOTHING
}

void TER_EdgeMatcher::SetId( size_t id )
{
    pathfinder_.SetId( id );
}

void TER_EdgeMatcher::SetChoiceRatio( float ratio )
{
    pathfinder_.SetChoiceRatio( ratio );
}

void TER_EdgeMatcher::SetConfiguration( unsigned refine, unsigned int subdivisions )
{
    pathfinder_.SetConfiguration( refine, subdivisions );
}

TER_EdgeMatcher::T_Waypoints TER_EdgeMatcher::FindWaypoints(
        const TER_EdgeMatcher::T_PathPoints& points, const geometry::Point2f& point )
{
    T_Waypoints result;
    for( size_t i = 0; i < points.size(); ++i )
        if( points[ i ].second >= 0 && points[ i ].first.Point().SquareDistance( point ) < squareEpsilon )
            result.push_back( std::make_pair( static_cast< int >( i ), points[ i ].second ) );
    return result;
}

std::pair< int, int > TER_EdgeMatcher::MatchWaypoints(
        const TER_EdgeMatcher::T_Waypoints& from, const TER_EdgeMatcher::T_Waypoints& to )
{
    if( !from.empty() && !to.empty() )
        for( auto it = from.begin(); it != from.end(); ++it )
        {
            const auto toWaypoint = std::find_if( to.begin(), to.end(),
                [&]( const T_Waypoints::value_type& value )
                {
                    return std::abs( value.second - it->second ) == 1;
                } );
            if( toWaypoint != to.end() )
                return std::make_pair( it->first, toWaypoint->first );
        }
    return std::make_pair( -1, -1 );
}

PathResultPtr TER_EdgeMatcher::ComputePath( const geometry::Point2f& from,
        const geometry::Point2f& to, TerrainRule_ABC& rule )
{
    const T_PathPoints points = ReadPathPoints( pathfind_.result() );
    const auto segment = MatchWaypoints( FindWaypoints( points, from ),
                                         FindWaypoints( points, to ) );
    if( segment.first < 0 || segment.second < 0 )
    {
        MT_LOG_INFO_MSG( "Segment [" << from << "] -> [" << to << "] not found in itinerary id='" << pathfind_.id() << "', computing a new path." );
        return pathfinder_.ComputePath( from, to, rule );
    }
    const auto res = boost::make_shared< PathResult >();
    res->found = false;
    res->points.reserve( segment.second - segment.first + 1 );
    for( int i = segment.first; i <= segment.second; ++i )
    {
        const auto& current = points[ i ];
        res->points.push_back( current.first );
        const auto& point = pathfind_.result().points().Get( i );
        res->found = point.waypoint() >= 0 && point.reached();
    }
    return res;
}

