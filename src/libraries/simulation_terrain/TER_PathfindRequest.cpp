// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathfindRequest.h"
#include "TER_PathComputer_ABC.h"
#include "TER_PathFinder_ABC.h"
#include "TER_Pathfinder.h"
#include "TER_World.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Vector2d.h"
#include <pathfind/TerrainPathPoint.h>
#include <tools/thread/Handler_ABC.h>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

TER_PathfindRequest::TER_PathfindRequest( TER_Pathfinder& manager,
        const boost::shared_ptr< TER_PathComputer_ABC >& computer,
        const sword::Pathfind& pathfind )
    : manager_( manager )
    , computer_( computer )
    , pathfind_( pathfind )
{
    // NOTHING
}

TER_PathfindRequest::~TER_PathfindRequest()
{
    // NOTHING
}

bool TER_PathfindRequest::IgnoreDynamicObjects() const
{
    return pathfind_.request().ignore_dynamic_objects();
}

namespace
{
    typedef std::vector< std::pair< TerrainPathPoint, int > > T_PathPoints;
    typedef std::vector< std::pair< int, int > > T_Waypoints;
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

    T_PathPoints ReadPathPoints( const sword::PathResult& result )
    {
        T_PathPoints points;
        BOOST_FOREACH( const auto& point, result.points() )
            points.push_back( std::make_pair( ReadPathPoint( point ), point.has_waypoint() ? point.waypoint() : -1 ) );
        return points;
    }

    struct PathfinderProxy : public TER_Pathfinder_ABC
                           , public boost::noncopyable
    {
        PathfinderProxy( TER_Pathfinder_ABC& pathfinder, const sword::Pathfind& pathfind )
            : pathfinder_( pathfinder )
            , pathfind_( pathfind )
        {
            // NOTHING
        }
        
        virtual void SetId( size_t id )
        {
            pathfinder_.SetId( id );
        }
        
        virtual void SetChoiceRatio( float ratio )
        {
            pathfinder_.SetChoiceRatio( ratio );
        }

        virtual void SetConfiguration( unsigned refine, unsigned int subdivisions )
        {
            pathfinder_.SetConfiguration( refine, subdivisions );
        }

        T_Waypoints FindWaypoints( const T_PathPoints& points, const geometry::Point2f& point )
        {
            T_Waypoints result;
            for( size_t i = 0; i < points.size(); ++i )
                if( points[ i ].second >= 0 && points[ i ].first.Point().SquareDistance( point ) < squareEpsilon )
                    result.push_back( std::make_pair( static_cast< int >( i ), points[ i ].second ) );
            return result;
        }

        std::pair< int, int > MatchWaypoints( const T_Waypoints& from, const T_Waypoints& to )
        {
            if( !from.empty() && !to.empty() )
                BOOST_FOREACH( const auto& fromWaypoint, from )
                {
                    const auto toWaypoint = std::find_if( to.begin(), to.end(), [&]( const T_Waypoints::value_type& value )
                    {
                        return std::abs( value.second - fromWaypoint.second ) == 1;
                    } );
                    if( toWaypoint != to.end() )
                        return std::make_pair( fromWaypoint.first, toWaypoint->first );
                }
            return std::make_pair( -1, -1 );
        }

        virtual PathResultPtr ComputePath( const geometry::Point2f& from,
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

    private:
        TER_Pathfinder_ABC& pathfinder_;
        const sword::Pathfind& pathfind_;
    };
}

double TER_PathfindRequest::FindPath( TER_Pathfinder_ABC& pathfinder )
{
    auto computer = computer_.lock(); // thread-safe
    if( !computer )
        return 0;
    MT_Profiler profiler;
    profiler.Start();
    if( IsItinerary() )
    {
        PathfinderProxy proxy( pathfinder, pathfind_ );
        computer->Execute( proxy );
    }
    else
    {
        computer->Execute( pathfinder );
    }
    return profiler.Stop();
}

bool TER_PathfindRequest::IsItinerary() const
{
    return pathfind_.has_result();
}
