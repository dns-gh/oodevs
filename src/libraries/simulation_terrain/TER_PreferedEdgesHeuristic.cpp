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
#include "TER_DynamicData.h"
#include "TER_PathIndex.h"
#include "TER_PathFinder_ABC.h"
#include "TER_PathFinderThread.h"
#include "TER_World.h"
#include "MT_Tools/MT_Vector2d.h"
#include <pathfind/TerrainRule_ABC.h>
#include <boost/make_shared.hpp>
#include "MT_Tools/MT_Logger.h"

namespace
{

// Relative value of actual costs of non-path edges.
const float costMultiplier = 20.f;
// Declare a new linear type to tag temporary itinerary graph segments 
const uint16_t itiLinear = 1 << 15;
const TerrainData itiTerrain = TerrainData( 0, 0, 0, itiLinear );

// FilterItiTerrain returns input terrain without the special itinerary linear bit.
TerrainData FilterItiTerrain( const TerrainData& t )
{
    return TerrainData( t.Area(), t.Left(), t.Right(), t.Linear() & ~itiTerrain.Linear() );
}

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

        cost *= costMultiplier;
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
        bool isIti = ( terrainBetween.Linear() & itiTerrain.Linear() ) == itiTerrain.Linear();
        const auto filteredBetween = FilterItiTerrain( terrainBetween );
        const auto filteredTo = FilterItiTerrain( terrainTo );
        if( isIti && reason )
            *reason << "itinerary\n";
        // For some reason probably related to the way the dynamic graph merge
        // linear data, we cannot rely on the itiTerrain to detect itinerary
        // edges overlapping existing edges with non-zero linear value (ie roads).
        // Fall back to geometry matching.
        for( auto it = indexes_.begin(); !isIti && it != indexes_.end(); ++it )
        {
            if( (*it)->IsPathEdge( from, to )  )
                isIti = true;
        }
        const auto cost = rule_.GetCost( from, to, filteredTo, filteredBetween, reason );
        if( cost < 0 || isIti )
            return cost;
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
        const std::vector< T_Itinerary >& itineraries,
        TER_PathFinderThread& graph )
    : pathfinder_( pathfinder )
    , itineraries_( itineraries )
    , graph_( graph )
{
    // Edges to be weighted have to exist in the graph in the first place.
    // Add the itineraries edges to the graph instance used to compute the
    // path and unregister them when done.
    for( auto it = itineraries_.begin(); it != itineraries_.end(); ++it )
    {
        T_PointVector points;
        for( auto ip = it->begin(); ip != it->end(); ++ip )
            points.push_back( MT_Vector2D( ip->X(), ip->Y() ) );
        const auto data = CreateRawDynamicData( points, itiTerrain );
        graph_.AddDynamicDataToRegister( data );
        registeredData_.push_back( data );
    }
}

TER_PreferedEdgesHeuristic::~TER_PreferedEdgesHeuristic()
{
    for( auto it = registeredData_.begin(); it != registeredData_.end(); ++it )
        graph_.AddDynamicDataToUnregister( *it );
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

