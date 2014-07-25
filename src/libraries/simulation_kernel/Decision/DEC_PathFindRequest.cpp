// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathFindRequest.h"
#include "DEC_PathFind_Manager_ABC.h"
#include "DEC_PathComputer_ABC.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_PathFinder_ABC.h"
#include "simulation_terrain/TER_World_ABC.h"
#include <pathfind/TerrainPathPoint.h>
#include <tools/thread/Handler_ABC.h>
#include <boost/foreach.hpp>

DEC_PathFindRequest::DEC_PathFindRequest( DEC_PathFind_Manager_ABC& manager, const boost::shared_ptr< DEC_PathComputer_ABC >& computer,
                                          const sword::Pathfind& pathfind, const TER_World_ABC& world )
    : manager_( manager )
    , computer_( computer )
    , pathfind_( pathfind )
    , world_( world )
{
    // NOTHING
}

DEC_PathFindRequest::~DEC_PathFindRequest()
{
    // NOTHING
}

bool DEC_PathFindRequest::IgnoreDynamicObjects() const
{
    return pathfind_.request().ignore_dynamic_objects();
}

namespace
{
    typedef std::vector< std::pair< TerrainPathPoint, int > > T_PathPoints;
    typedef std::vector< std::pair< int, int > > T_Waypoints;

    TerrainData ReadTerrainData( const sword::TerrainData& data )
    {
        return TerrainData( static_cast< unsigned char >( data.area() ),
                            static_cast< unsigned char >( data.left() ),
                            static_cast< unsigned char >( data.right() ),
                            static_cast< unsigned short >( data.linear() ) );
    }

    TerrainPathPoint ReadPathPoint( const sword::PathPoint& point, const TER_World_ABC& world )
    {
        MT_Vector2D position;
        world.MosToSimMgrsCoord( point.coordinate().latitude(), point.coordinate().longitude(), position );
        return TerrainPathPoint( geometry::Point2f( static_cast< float >( position.GetX() ),
                                                    static_cast< float >( position.GetY() ) ),
                                 ReadTerrainData( point.current() ), ReadTerrainData( point.next() ) );
    }

    T_PathPoints ReadPathPoints( const sword::PathResult& result, const TER_World_ABC& world )
    {
        T_PathPoints points;
        BOOST_FOREACH( const auto& point, result.points() )
            points.push_back( std::make_pair( ReadPathPoint( point, world ), point.waypoint() ) );
        return points;
    }

    struct PathfinderProxy : public TER_Pathfinder_ABC
                           , public boost::noncopyable
    {
        PathfinderProxy( TER_Pathfinder_ABC& pathfinder, const sword::Pathfind& pathfind, const TER_World_ABC& world )
            : pathfinder_( pathfinder )
            , pathfind_( pathfind )
            , world_( world )
        {
            // NOTHING
        }
        
        virtual void SetId( size_t id )
        {
            pathfinder_.SetId( id );
        }
        
        virtual void SetCallback( pathfind::AStarManagementCallback_ABC* callback )
        {
            pathfinder_.SetCallback( callback );
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
            for( int i = 0; i < points.size(); ++i )
                if( points[ i ].first.Point().SquareDistance( point ) < 0.01 )
                    result.push_back( std::make_pair( i, points[ i ].second ) );
            return result;
        }

        std::pair< int, int > MatchWaypoints( const T_Waypoints& from, const T_Waypoints& to )
        {
            if( !from.empty() && !to.empty() )
                BOOST_FOREACH( const auto& fromWaypoint, from )
                {
                    const auto toWaypoint = std::find_if( to.begin(), to.end(), [&]( const T_Waypoints::value_type& value )
                    {
                        return value.second == ( fromWaypoint.second + 1 );
                    } );
                    if( toWaypoint != to.end() )
                        return std::make_pair( fromWaypoint.first, toWaypoint->first );
                }
            return std::make_pair( -1, -1 );
        }

        virtual bool ComputePath( const geometry::Point2f& from, const geometry::Point2f& to,
                                  TerrainRule_ABC& rule,
                                  tools::thread::Handler_ABC< TerrainPathPoint >& handler )
        {
            const T_PathPoints points = ReadPathPoints( pathfind_.result(), world_ );
            std::pair< int, int > segment = MatchWaypoints( FindWaypoints( points, from ),
                                                            FindWaypoints( points, to ) );
            if( segment.first < 0 || segment.second < 0 )
                return pathfinder_.ComputePath( from, to, rule, handler );
            bool reached = false;
            for( int i = segment.first; i <= segment.second; ++i )
            {
                const auto& current = points[ i ];
                handler.Handle( current.first );
                const auto& point = pathfind_.result().points().Get( i );
                reached = point.waypoint() >= 0 && point.reached();
            }
            return reached;
        }

    private:
        TER_Pathfinder_ABC& pathfinder_;
        const sword::Pathfind& pathfind_;
        const TER_World_ABC& world_;
    };
}

void DEC_PathFindRequest::FindPath( TER_Pathfinder_ABC& pathfinder )
{
    auto computer = computer_.lock(); // thread-safe
    if( !computer )
        return;
    MT_Profiler profiler;
    profiler.Start();
    if( IsItinerary() )
    {
        PathfinderProxy proxy( pathfinder, pathfind_, world_ );
        computer->Execute( proxy );
    }
    else
    {
        computer->Execute( pathfinder );
    }
    manager_.CleanPathAfterComputation( profiler.Stop() );
}

bool DEC_PathFindRequest::IsItinerary() const
{
    return pathfind_.has_result();
}
