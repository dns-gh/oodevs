// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathfindRequest.h"

TER_PathfindRequest::TER_PathfindRequest(
        const boost::shared_ptr< TER_PathComputer_ABC >& computer,
        const sword::Pathfind& pathfind )
    : computer_( computer )
    , pathfind_( pathfind )
{
    // NOTHING
}

TER_PathfindRequest::~TER_PathfindRequest()
{
    // NOTHING
}

bool TER_PathfindRequest::IgnoreDynamicObjects() const
{
    return pathfind_.request().ignore_dynamic_objects();
}

boost::shared_ptr< TER_PathComputer_ABC > TER_PathfindRequest::GetComputer()
{
    return computer_.lock();
}

const sword::Pathfind& TER_PathfindRequest::GetPathfind() const
{
    return pathfind_;
}

bool TER_PathfindRequest::IsItinerary() const
{
    return pathfind_.has_result();
}
