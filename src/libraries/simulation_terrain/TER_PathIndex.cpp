// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathIndex.h"
#include <boost/functional/hash.hpp>
#include <cmath>
#include <limits>
#include <unordered_set>

namespace
{

// Returns the index and distance of the path point the closest to p.
// Note: an improved version may consider points on edges instead of only node
// points.
// Note: this implementation is horribly inefficient, but good enough in the
// narrow context of logistic paths. Using a spatial index would be a major
// improvement.
std::pair< float, size_t > DistanceToPathPoints( geometry::Point2f p,
        const std::vector< geometry::Point2f >& path )
{
    float distance = -1;
    size_t index = 0;
    for( size_t i = 0; i < path.size(); ++i )
    {
        const float d = path[i].SquareDistance( p );
        if( d < distance || distance < 0 )
        {
            distance = d;
            index = i;
        }
    }
    return std::make_pair( std::sqrt( distance ), index );
}

struct Point
{
    uint32_t x;
    uint32_t y;

    bool operator==( Point other ) const
    {
        return x == other.x && y == other.y;
    }
};

Point MakePoint( geometry::Point2f point )
{
    Point p;
    static const float precision = 3.f;
    p.x = static_cast< uint32_t >( point.X() / precision );
    p.y = static_cast< uint32_t >( point.Y() / precision );
    return p;
}

struct Edge
{
    Point p1;
    Point p2;

    bool operator==( Edge other ) const
    {
        return p1 == other.p1 && p2 == other.p2;
    }
};

// Gridded and normalized edge to make it hashable.
Edge MakeEdge( geometry::Point2f p1, geometry::Point2f p2 )
{
    if( p1.X() > p2.X() || p1.X() == p2.X() && p1.Y() > p2.Y() )
        std::swap( p1, p2 );
    Edge e;
    e.p1 = MakePoint( p1 );
    e.p2 = MakePoint( p2 );
    return e;
}

struct EdgeHasher
{
    size_t operator()( Edge e ) const
    {
        size_t h = 0;
        boost::hash_combine( h, e.p1.x );
        boost::hash_combine( h, e.p1.y );
        boost::hash_combine( h, e.p2.x );
        boost::hash_combine( h, e.p2.y );
        return h;
    }
};

} // namespace

struct TER_PathIndex::Private
{
    // Path edges, snapped on a grid to reduce the effect of floating point
    // noise.
    std::unordered_set< Edge, EdgeHasher > edges;
};

TER_PathIndex::TER_PathIndex( const std::vector< geometry::Point2f >& path,
                              geometry::Point2f destination )
    : path_( path )
    , bestIndex_( 0 )
    , distanceToDest_( std::numeric_limits< float >::infinity() )
    , private_( new Private() )
{
    if( path_.empty() )
        throw MASA_EXCEPTION( "invalid empty path" );
    const auto d = DistanceToPathPoints( destination, path );
    distanceToDest_ = d.first;
    bestIndex_ = d.second;

    distances_.resize( path_.size() );
    for( size_t i = bestIndex_; i > 0; --i )
    {
        const auto d = path_[i].Distance( path_[i-1] );
        distances_[i-1] = distances_[i] + d;
    }
    for( size_t i = bestIndex_ + 1; i < path_.size(); ++i )
    {
        const auto d = path_[i].Distance( path_[i-1] );
        distances_[i] = distances_[i-1] + d;
    }

    for( size_t i = 1; i < path_.size(); ++i )
        private_->edges.insert( MakeEdge( path_[i-1], path_[i] ));
}

TER_PathIndex::~TER_PathIndex()
{
}

TER_PathIndex::Distance TER_PathIndex::GetDistanceUsingPath( geometry::Point2f point ) const
{
    const auto startToPath = DistanceToPathPoints( point, path_ );
    Distance d;
    d.startToPath = startToPath.first;
    d.alongPath = distances_[ startToPath.second ];
    d.pathToDest = distanceToDest_;
    return d;
}

bool TER_PathIndex::IsPathEdge( geometry::Point2f p1, geometry::Point2f p2 ) const
{
    return private_->edges.count( MakeEdge( p1, p2 ) ) > 0;
}
