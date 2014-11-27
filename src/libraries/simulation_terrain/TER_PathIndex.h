// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHINDEX_H
#define SIMULATION_TERRAIN_PATHINDEX_H

#include <geometry/Types.h>
#include <boost/noncopyable.hpp>
#include <vector>

// TER_PathIndex indexes a path relatively to a destination point and can
// answer queries about distances to the destination or points path ownership.
class TER_PathIndex : private boost::noncopyable
{
public:
    struct Distance
    {
        float startToPath;
        float alongPath;
        float pathToDest;
    };

    // Throws if path has less than two points.
    TER_PathIndex( const std::vector< geometry::Point2f >& path,
                   geometry::Point2f destination );
    ~TER_PathIndex();

    // Returns a tuple describing the distance of point to path, distance
    // along the path of the closest point on path to point and closest path
    // to destination and distance from closest point to destination and
    // destination.
    Distance GetDistanceUsingPath( const geometry::Point2f& point ) const;

    // Returns true if [p1, p2] edge belongs to the path.
    bool IsPathEdge( const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;

private:
    struct Indexes;

private:
    const std::vector< geometry::Point2f > path_;
    // Distance of each path point along the path to the point closest to
    // the destination.
    std::vector< float > distances_;
    // Distance of the destination to the path point the closest to it
    float distanceToDest_;
    std::unique_ptr< Indexes > indexes_;
};

#endif // SIMULATION_TERRAIN_PATHINDEX_H
