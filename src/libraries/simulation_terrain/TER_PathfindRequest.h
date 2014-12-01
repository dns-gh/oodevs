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

#include <geometry/Types.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class TER_PathIndex;
class TER_PathSection;

// TER_PathfindRequest defines possible path finding arguments. Fill it and
// pass it to TER_Pathfinder to compute a path.
class TER_PathfindRequest: private boost::noncopyable
{
public:
    typedef std::vector< geometry::Point2f > Itinerary;

             TER_PathfindRequest(
                std::size_t callerId,
                const std::vector< boost::shared_ptr< TER_PathSection > > sections );
    virtual ~TER_PathfindRequest();

    bool IgnoreDynamicObjects() const;
    void SetIgnoreDynamicObjects( bool ignore );
    void SetItinerary( const Itinerary& itinerary );
    const Itinerary& GetItinerary() const;

    std::size_t GetCallerId() const;
    const std::vector< boost::shared_ptr< TER_PathSection > >& GetSections();
    double GetLength() const;

private:

    const size_t callerId_;
    const std::vector< boost::shared_ptr< TER_PathSection > > sections_;
    bool ignoreDynamicObjects_;
    Itinerary itinerary_;
};

#endif // SIMULATION_TERRAIN_PATHFINDREQUEST_H

