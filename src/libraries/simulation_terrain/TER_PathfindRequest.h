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

class TER_PathSection;

// TER_PathfindRequest defines possible path finding arguments. Fill it and
// pass it to TER_Pathfinder to compute a path.
class TER_PathfindRequest: private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathfindRequest(
                std::size_t callerId,
                const std::vector< boost::shared_ptr< TER_PathSection > > sections,
                const sword::Pathfind& pathfind );
    virtual ~TER_PathfindRequest();
    //@}

    bool IgnoreDynamicObjects() const;
    bool IsItinerary() const;

    std::size_t GetCallerId() const;
    const sword::Pathfind& GetPathfind() const;
    const std::vector< boost::shared_ptr< TER_PathSection > >& GetSections();
    double GetLength() const;

private:
    //! @name Member data
    //@{
    const size_t callerId_;
    const std::vector< boost::shared_ptr< TER_PathSection > > sections_;
    const sword::Pathfind pathfind_;
    //@}
};

#endif // SIMULATION_TERRAIN_PATHFINDREQUEST_H

