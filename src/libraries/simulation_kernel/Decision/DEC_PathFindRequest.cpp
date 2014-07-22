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
#include "DEC_PathFind_Manager.h"
#include "DEC_PathComputer_ABC.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_PathFinder_ABC.h"
#include <pathfind/TerrainPathPoint.h>
#include <tools/thread/Handler_ABC.h>

DEC_PathFindRequest::DEC_PathFindRequest( DEC_PathFind_Manager& m, const boost::shared_ptr< DEC_PathComputer_ABC >& p,
                                          bool ignoreDynamicObjects, const sword::Pathfind* pathfind )
    : manager_( m )
    , path_( p )
    , ignoreDynamicObjects_( ignoreDynamicObjects )
    , pathfind_( pathfind ? new sword::Pathfind( *pathfind ) : nullptr )
{
    // NOTHING
}

DEC_PathFindRequest::~DEC_PathFindRequest()
{
    // NOTHING
}

bool DEC_PathFindRequest::IgnoreDynamicObjects() const
{
    return ignoreDynamicObjects_;
}

namespace
{
    TerrainData Convert( const sword::TerrainData& data )
    {
        return TerrainData( static_cast< unsigned char >( data.area() ),
                            static_cast< unsigned char >( data.left() ),
                            static_cast< unsigned char >( data.right() ),
                            static_cast< unsigned short >( data.linear() ) );
    }

    TerrainPathPoint Convert( const sword::PathPoint& point )
    {
        MT_Vector2D position;
        MIL_Tools::ConvertCoordMosToSim( point.coordinate(), position );
        return TerrainPathPoint( geometry::Point2f( static_cast< float >( position.GetX() ),
                                                    static_cast< float >( position.GetY() ) ),
                                 Convert( point.current() ), Convert( point.next() ) );
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

        std::pair< int, int > FindSegment( const geometry::Point2f& from, const geometry::Point2f& to ) const
        {
            int beginIndex = INT_MIN;
            int beginWaypoint = INT_MIN;
            int endIndex = INT_MIN;
            int index = 0;
            int waypoint = -1;
            const auto& points = pathfind_.result().points();
            for( auto it = points.begin(); it != points.end(); ++it, ++index )
            {
                if( it->waypoint() < 0 )
                    continue;
                ++waypoint;
                const auto current = Convert( *it );
                if( current.Point().SquareDistance( from ) < 1 )
                {
                    beginIndex = index;
                    beginWaypoint = waypoint;
                }
                if( current.Point().SquareDistance( to ) < 1 )
                    endIndex = index;
                if( beginWaypoint + 1 == waypoint && endIndex == index )
                    return std::make_pair( beginIndex, endIndex );
            }
            return std::make_pair( beginIndex, endIndex );
        }

        virtual bool ComputePath( const geometry::Point2f& from, const geometry::Point2f& to,
                                  TerrainRule_ABC& rule,
                                  tools::thread::Handler_ABC< TerrainPathPoint >& handler )
        {
            auto& points = pathfind_.result().points();
            const auto segment = FindSegment( from, to );
            if( segment.first < 0 || segment.second < 0 )
                return pathfinder_.ComputePath( from, to, rule, handler );
            bool reached = false;
            for( int i = segment.first; i <= segment.second; ++i )
            {
                const auto& current = points.Get( i );
                const auto zorg = Convert( current );
                handler.Handle( zorg );
                reached = current.waypoint() > 0 && current.reached();
            }
            return reached;
        }

    private:
        TER_Pathfinder_ABC& pathfinder_;
        const sword::Pathfind& pathfind_;
    };
}

void DEC_PathFindRequest::FindPath( TER_Pathfinder_ABC& pathfinder )
{
    auto path = path_.lock(); // thread-safe
    if( !path )
        return;
    MT_Profiler profiler;
    profiler.Start();
    if( IsItinerary() )
    {
        PathfinderProxy proxy( pathfinder, *pathfind_ );
        path->Execute( proxy );
    }
    else
    {
        path->Execute( pathfinder );
    }
    manager_.CleanPathAfterComputation( profiler.Stop() );
}

bool DEC_PathFindRequest::IsItinerary() const
{
    return pathfind_ && pathfind_->has_result();
}
