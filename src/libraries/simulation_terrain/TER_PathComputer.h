// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHCOMPUTER_H
#define SIMULATION_TERRAIN_PATHCOMPUTER_H

#include "MT_Tools/MT_Profiler.h"
#include "TER_Path_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

struct PathResult;
class TER_PathFuture;
class TER_PathPoint;
class TER_PathSection;
struct TER_PathResult;

// =============================================================================
/** @class  TER_PathComputer
    @brief  TER_PathComputer
*/
// Created: MCO 2014-05-15
// =============================================================================
class TER_PathComputer : private boost::noncopyable
{
public:
    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;

public:
             TER_PathComputer();
    virtual ~TER_PathComputer();

    boost::shared_ptr< TER_PathResult > Execute(
        std::size_t requestId, std::size_t callerId,
        const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
        TER_Pathfinder_ABC& pathfind, TER_PathFuture& future,
        unsigned int deadlineSeconds, bool debugPath );

private:
    void DoExecute( const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
            TER_Pathfinder_ABC& pathfind, TER_PathFuture& future,
            unsigned int deadlineSeconds );
    boost::shared_ptr< PathResult > ComputeSection( TER_Pathfinder_ABC& pathfinder,
        TER_PathSection& section, TER_PathFuture& future,
        unsigned int deadlineSeconds );
    void NotifyPartialSection();
    void NotifyCompletedSection();
    boost::optional< MT_Vector2D > GetLastPosition() const;
    boost::shared_ptr< TER_PathResult > GetPathResult() const;

private:
    MT_Profiler profiler_;
    TER_Path_ABC::E_State nState_;
    MT_Vector2D lastWaypoint_;
    std::vector< MT_Vector2D > computedWaypoints_;
    T_PathPoints resultList_;
};

#endif // SIMULATION_TERRAIN_PATHCOMPUTER_H
