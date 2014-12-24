// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PreferedEdgesHeuristic.h"
#include "TER_PathIndex.h"
#include "TER_PathFinder_ABC.h"
#include "TER_World.h"
#include "MT_Tools/MT_Vector2d.h"
#include <pathfind/TerrainRule_ABC.h>
#include <boost/make_shared.hpp>
#include "MT_Tools/MT_Logger.h"

namespace
{

// Relative value of actual costs of non-path edges.
static const float costMultiplier = 20.f;

class TER_PreferedEdges : public TerrainRule_ABC
{
public:
    TER_PreferedEdges( const std::vector< std::vector< geometry::Point2f > >& paths,
                       TerrainRule_ABC& rule,
                       geometry::Point2f to )
        : rule_( rule )
    {
        for( auto it = paths.begin(); it != paths.end(); ++it )
            indexes_.push_back( std::unique_ptr< TER_PathIndex >( new TER_PathIndex( *it, to ) ));
        const geometry::Point2f p1( 0, 0 );
        const geometry::Point2f p2( 10000, 0 );
        speedFactor_ = rule_.EvaluateCost( p1, p2 ) / p1.Distance( p2 );
    }

    virtual float EvaluateCost( const geometry::Point2f& from,
                                const geometry::Point2f& to )
    {
        auto cost = rule_.EvaluateCost( from, to );
        if( cost <= 0 )
            return cost;

        for( auto it = indexes_.begin(); it != indexes_.end(); ++it )
        {
            const auto dist = (*it)->GetDistanceUsingPath( to );
            auto pathCost = costMultiplier*dist.startToPath
                + dist.alongPath
                + costMultiplier*dist.pathToDest;
            pathCost *= speedFactor_;
            if( pathCost < cost )
                cost = pathCost;
        }
        return cost;
    }

    virtual float GetCost( const geometry::Point2f& from,
                           const geometry::Point2f& to,
                           const TerrainData& terrainTo,
                           const TerrainData& terrainBetween,
                           std::ostream* reason )
    {
        const auto cost = rule_.GetCost( from, to, terrainTo, terrainBetween, reason );
        if( cost < 0 )
            return cost;
        for( auto it = indexes_.begin(); it != indexes_.end(); ++it )
        {
            if( (*it)->IsPathEdge( from, to )  )
                return cost;
        }
        return costMultiplier*cost;
    }

    virtual bool ShouldEndComputation()
    {
        return rule_.ShouldEndComputation();
    }

private:
    std::vector< std::unique_ptr< TER_PathIndex > > indexes_;
    TerrainRule_ABC& rule_;
    float speedFactor_;
};

}  // namespace

TER_PreferedEdgesHeuristic::TER_PreferedEdgesHeuristic(
        const boost::shared_ptr< TER_Pathfinder_ABC >& pathfinder,
        const std::vector< T_Itinerary >& itineraries )
    : pathfinder_( pathfinder )
    , itineraries_( itineraries )
{
}

void TER_PreferedEdgesHeuristic::SetChoiceRatio( float ratio )
{
    pathfinder_->SetChoiceRatio( ratio );
}

void TER_PreferedEdgesHeuristic::SetConfiguration( unsigned refine, unsigned int subdivisions )
{
    pathfinder_->SetConfiguration( refine, subdivisions );
}

PathResultPtr TER_PreferedEdgesHeuristic::ComputePath( const geometry::Point2f& from,
        const geometry::Point2f& to, TerrainRule_ABC& rule )
{
    TER_PreferedEdges heuristic( itineraries_, rule, to );
    return pathfinder_->ComputePath( from, to, heuristic );
}

