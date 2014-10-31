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

#include "TER_PathFindRequest_ABC.h"
#include <protocol/Simulation.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace sword
{
    class Pathfind;
}

class TER_PathComputer_ABC;
class TER_PathfindManager2;

// TER_PathfindRequest insulates TER_Path_ABC cleanup logic from
// TER_PathFinderThread. While the cleanup code itself belongs to TER_Path_ABC,
// the deferred cleanup logic is only required by the threaded implementation
// of the path finder and has nothing to do in TER_Path_ABC. We need both a
// request and an executor class.
class TER_PathfindRequest: public TER_PathFindRequest_ABC,
                           private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathfindRequest( TER_PathfindManager2& manager,
                const boost::shared_ptr< TER_PathComputer_ABC >& computer,
                const sword::Pathfind& pathfind );
    virtual ~TER_PathfindRequest();
    //@}

    //! @name Operations
    //@{
    virtual bool IgnoreDynamicObjects() const;
    virtual void FindPath( TER_Pathfinder_ABC& pathfind );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsItinerary() const;
    //@}

private:
    //! @name Member data
    //@{
    TER_PathfindManager2& manager_;
    boost::weak_ptr< TER_PathComputer_ABC > computer_;
    const sword::Pathfind pathfind_;
    //@}
};

#endif // SIMULATION_TERRAIN_PATHFINDREQUEST_H

