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
#include "TER_EdgeMatcher.h"
#include "TER_PathComputer_ABC.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/make_shared.hpp>

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

double TER_PathfindRequest::FindPath( TER_Pathfinder_ABC& pathfinder, unsigned int deadline )
{
    auto computer = computer_.lock(); // thread-safe
    if( !computer )
        return 0;
    MT_Profiler profiler;
    profiler.Start();
    if( IsItinerary() )
    {
        TER_EdgeMatcher matcher( pathfinder, pathfind_ );
        computer->Execute( matcher, deadline );
    }
    else
    {
        computer->Execute( pathfinder, deadline );
    }
    return profiler.Stop();
}

bool TER_PathfindRequest::IsItinerary() const
{
    return pathfind_.has_result();
}
