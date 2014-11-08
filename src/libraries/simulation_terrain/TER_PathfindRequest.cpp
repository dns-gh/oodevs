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
#include "TER_PathSection.h"

TER_PathfindRequest::TER_PathfindRequest(
        const std::vector< boost::shared_ptr< TER_PathSection > > sections,
        const boost::shared_ptr< TER_PathComputer_ABC >& computer,
        const sword::Pathfind& pathfind,
        const boost::shared_ptr< TER_PathFuture >& future )
    : sections_( sections )
    , computer_( computer )
    , pathfind_( pathfind )
    , future_( future )
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

boost::shared_ptr< TER_PathFuture > TER_PathfindRequest::GetFuture()
{
    return future_;
}

bool TER_PathfindRequest::IsItinerary() const
{
    return pathfind_.has_result();
}

const std::vector< boost::shared_ptr< TER_PathSection > >& TER_PathfindRequest::GetSections()
{
    return sections_;
}

double TER_PathfindRequest::GetLength() const
{
   double length = 0;
   for( auto it = sections_.begin(); it != sections_.end(); ++it )
      length += (*it)->GetPosStart().Distance( (*it)->GetPosEnd() );
   return length;
}
