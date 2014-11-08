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

class TER_PathFuture;
class TER_PathComputer_ABC;
class TER_PathSection;

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
             TER_PathfindRequest(
                const std::vector< boost::shared_ptr< TER_PathSection > > sections,
                const boost::shared_ptr< TER_PathComputer_ABC >& computer,
                const sword::Pathfind& pathfind,
                const boost::shared_ptr< TER_PathFuture >& future );
    virtual ~TER_PathfindRequest();
    //@}

    bool IgnoreDynamicObjects() const;
    bool IsItinerary() const;

    // Returns the path computer or 0 if it was destroyed before the request
    // could be processed.
    boost::shared_ptr< TER_PathComputer_ABC > GetComputer();
    const sword::Pathfind& GetPathfind() const;
    boost::shared_ptr< TER_PathFuture > GetFuture();
    const std::vector< boost::shared_ptr< TER_PathSection > >& GetSections();
    double GetLength() const;

private:
    //! @name Member data
    //@{
    const std::vector< boost::shared_ptr< TER_PathSection > > sections_;
    boost::shared_ptr< TER_PathComputer_ABC > computer_;
    const sword::Pathfind pathfind_;
    boost::shared_ptr< TER_PathFuture > future_;
    //@}
};

#endif // SIMULATION_TERRAIN_PATHFINDREQUEST_H

