// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHFINDREQUEST_H
#define SIMULATION_TERRAIN_PATHFINDREQUEST_H

#include <protocol/Simulation.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace sword
{
    class Pathfind;
}

class TER_PathComputer_ABC;
class TER_Pathfinder_ABC;
class TER_Pathfinder;

// TER_PathfindRequest insulates TER_Path_ABC cleanup logic from
// TER_PathFinderThread. While the cleanup code itself belongs to TER_Path_ABC,
// the deferred cleanup logic is only required by the threaded implementation
// of the path finder and has nothing to do in TER_Path_ABC. We need both a
// request and an executor class.
class TER_PathfindRequest: private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathfindRequest( TER_Pathfinder& manager,
                const boost::shared_ptr< TER_PathComputer_ABC >& computer,
                const sword::Pathfind& pathfind );
    virtual ~TER_PathfindRequest();
    //@}

    bool IgnoreDynamicObjects() const;

    // Computes path and returned processing time in milliseconds.
    double FindPath( TER_Pathfinder_ABC& pathfind, unsigned int deadline );

private:
    //! @name Helpers
    //@{
    bool IsItinerary() const;
    //@}

private:
    //! @name Member data
    //@{
    TER_Pathfinder& manager_;
    boost::weak_ptr< TER_PathComputer_ABC > computer_;
    const sword::Pathfind pathfind_;
    //@}
};

#endif // SIMULATION_TERRAIN_PATHFINDREQUEST_H

