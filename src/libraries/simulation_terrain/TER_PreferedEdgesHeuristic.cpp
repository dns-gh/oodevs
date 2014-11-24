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
#include "protocol/Simulation.h"
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
    TER_PreferedEdges( const std::vector< geometry::Point2f >& path,
                       TerrainRule_ABC& rule,
                       geometry::Point2f to )
        : rule_( rule )
    {
        if( !path.empty() )
            index_.reset( new TER_PathIndex( path, to ) );
        const geometry::Point2f p1( 0, 0 );
        const geometry::Point2f p2( 10000, 0 );
        speedFactor_ = rule_.EvaluateCost( p1, p2 ) / p1.Distance( p2 );
    }

    virtual float EvaluateCost( const geometry::Point2f& from,
                                const geometry::Point2f& to )
    {
        const auto cost = rule_.EvaluateCost( from, to );
        if( !index_ || cost <= 0 )
            return cost;
        const auto dist = index_->GetDistanceUsingPath( to );
        const auto pathCost = costMultiplier*dist.startToPath
            + dist.alongPath
            + costMultiplier*dist.pathToDest;
        return std::min( cost, speedFactor_*pathCost );
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
        if( index_->IsPathEdge( from, to )  )
            return cost;
        return costMultiplier*cost;
    }

    virtual bool ShouldEndComputation()
    {
        return rule_.ShouldEndComputation();
    }

private:
    std::unique_ptr< TER_PathIndex > index_;
    TerrainRule_ABC& rule_;
    float speedFactor_;
};

}  // namespace

TER_PreferedEdgesHeuristic::TER_PreferedEdgesHeuristic(
        const boost::shared_ptr< TER_Pathfinder_ABC >& pathfinder,
        const sword::Pathfind& pathfind )
    : pathfinder_( pathfinder )
{
    if( !pathfind.has_result() )
        return;
    const auto& points = pathfind.result().points();
    for( int i = 0; i < points.size(); ++i )
    {
        MT_Vector2D current;
        TER_World::GetWorld().MosToSimMgrsCoord(
                points.Get( i ).coordinate().latitude(),
                points.Get( i ).coordinate().longitude(),
                current );
        geometry::Point2f p( static_cast< float >( current.rX_ ),
                             static_cast< float >( current.rY_ ) );
        points_.push_back( p );
    }
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
    TER_PreferedEdges heuristic( points_, rule, to );
    return pathfinder_->ComputePath( from, to, heuristic );
}

