// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHCOMPUTER_ABC
#define SIMULATION_TERRAIN_PATHCOMPUTER_ABC

#include "TER_Path_ABC.h"
#include "TER_PathResult_ABC.h"
#include <boost/noncopyable.hpp>

class MT_Vector2D;
class TER_PathPoint;
class TER_PathSection;
class TER_Pathfinder_ABC;

// =============================================================================
/** @class  TER_PathComputer_ABC
    @brief  TER_PathComputer_ABC
*/
// Created: MCO 2014-05-15
// =============================================================================
class TER_PathComputer_ABC : public TER_PathResult_ABC
{
public:
             TER_PathComputer_ABC() {}
    virtual ~TER_PathComputer_ABC() {}

    virtual double GetLength() const = 0;
    virtual void Execute( TER_Pathfinder_ABC& pathfind ) = 0;
    virtual void Cancel() = 0;
    virtual TER_Path_ABC::E_State GetState() const = 0;
    virtual void RegisterPathSection( TER_PathSection& section ) = 0;

    virtual const MT_Vector2D& GetLastWaypoint() const = 0;
    virtual const std::vector< MT_Vector2D >& GetComputedWaypoints() const = 0;
    virtual void RemoveComputedWaypoint() = 0;

    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;
    virtual T_PathPoints GetResult() const = 0;

};

#endif // SIMULATION_TERRAIN_PATHCOMPUTER_ABC
