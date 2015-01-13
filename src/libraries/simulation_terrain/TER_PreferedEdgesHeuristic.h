// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PREFEREDEDGESHEURISTIC_H
#define SIMULATION_TERRAIN_PREFEREDEDGESHEURISTIC_H

#include "TER_Pathfinder_ABC.h"
#include <geometry/Types.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class TerrainRule_ABC;
class TER_DynamicData;
class TER_PathFinderThread;
typedef boost::shared_ptr< TER_DynamicData > DynamicDataPtr;

namespace sword
{
    class Pathfind;
}

// TER_PreferedEdgesHeuristic inserts prefered itineraries edges into the
// pathfind grpah and adjusts edge costs to lower those of known edges defined
// in a path result, and favor such path.
class TER_PreferedEdgesHeuristic : public TER_Pathfinder_ABC
                                 , private boost::noncopyable
{
public:
    typedef std::vector< geometry::Point2f > T_Itinerary;

    TER_PreferedEdgesHeuristic( const boost::shared_ptr< TER_Pathfinder_ABC >& pathfinder,
                                const std::vector< T_Itinerary >& itineraries,
                                TER_PathFinderThread& graph );
    ~TER_PreferedEdgesHeuristic();
    
    virtual void SetChoiceRatio( float ratio );
    virtual void SetConfiguration( unsigned refine, unsigned int subdivisions );
    virtual PathResultPtr ComputePath( const geometry::Point2f& from,
            const geometry::Point2f& to, TerrainRule_ABC& rule );

private:
    boost::shared_ptr< TER_Pathfinder_ABC > pathfinder_;
    std::vector< T_Itinerary > itineraries_;
    // The graph used for path computation
    TER_PathFinderThread& graph_;
    // Dynamic data created from itineraries points and registered in the
    // graph until the computation ends.
    std::vector< DynamicDataPtr > registeredData_;
};

#endif // SIMULATION_TERRAIN_PREFEREDEDGESHEURISTIC_H

