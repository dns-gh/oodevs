// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathComputer_h_
#define __DEC_PathComputer_h_

#include "MT_Tools/MT_Profiler.h"
#include "simulation_terrain/TER_PathComputer_ABC.h"
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

struct PathResult;
class TER_PathFuture;
class TER_PathPoint;
struct TER_PathResult;

// =============================================================================
/** @class  DEC_PathComputer
    @brief  DEC_PathComputer
*/
// Created: MCO 2014-05-15
// =============================================================================
class DEC_PathComputer : public TER_PathComputer_ABC
{
public:
    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;

public:
             DEC_PathComputer( std::size_t id );
    virtual ~DEC_PathComputer();

    virtual boost::shared_ptr< TER_PathResult > Execute(
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
    boost::optional< MT_Vector2D > DEC_PathComputer::GetLastPosition() const;
    boost::shared_ptr< TER_PathResult > GetPathResult() const;

private:
    MT_Profiler profiler_;
    std::size_t id_;
    // Identify the current computation, for logging purpose
    const std::size_t computerId_;
    TER_Path_ABC::E_State nState_;
    MT_Vector2D lastWaypoint_;
    std::vector< MT_Vector2D > computedWaypoints_;
    T_PathPoints resultList_;
};

#endif // __DEC_PathComputer_h_
