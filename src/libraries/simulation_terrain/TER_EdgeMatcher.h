// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_EDGEMATCHER_H
#define SIMULATION_TERRAIN_EDGEMATCHER_H

#include "TER_Pathfinder_ABC.h"
#include <pathfind/TerrainPathPoint.h>
#include <boost/noncopyable.hpp>
#include <vector>

class TerrainRule_ABC;

namespace sword
{
    class Pathfind;
}

class TER_EdgeMatcher : public TER_Pathfinder_ABC
                      , private boost::noncopyable
{
public:
    typedef std::vector< std::pair< TerrainPathPoint, int > > T_PathPoints;
    typedef std::vector< std::pair< int, int > > T_Waypoints;

public:
    TER_EdgeMatcher( TER_Pathfinder_ABC& pathfinder, const sword::Pathfind& pathfind );
    
    virtual void SetId( size_t id );
    virtual void SetChoiceRatio( float ratio );
    virtual void SetConfiguration( unsigned refine, unsigned int subdivisions );
    virtual PathResultPtr ComputePath( const geometry::Point2f& from,
            const geometry::Point2f& to, TerrainRule_ABC& rule );

private:
    T_Waypoints FindWaypoints( const T_PathPoints& points,
            const geometry::Point2f& point );
    std::pair< int, int > MatchWaypoints( const T_Waypoints& from,
            const T_Waypoints& to );
private:
    TER_Pathfinder_ABC& pathfinder_;
    const sword::Pathfind& pathfind_;
};

#endif // SIMULATION_TERRAIN_EDGEMATCHER_H

