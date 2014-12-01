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
        std::size_t callerId,
        const std::vector< boost::shared_ptr< TER_PathSection > > sections )
    : callerId_( callerId )
    , sections_( sections )
    , ignoreDynamicObjects_( false )
{
    // NOTHING
}

TER_PathfindRequest::~TER_PathfindRequest()
{
    // NOTHING
}

void TER_PathfindRequest::SetIgnoreDynamicObjects( bool ignore )
{
    ignoreDynamicObjects_ = ignore;
}

bool TER_PathfindRequest::IgnoreDynamicObjects() const
{
    return ignoreDynamicObjects_;
}

void TER_PathfindRequest::SetItinerary( const TER_PathfindRequest::Itinerary& itinerary )
{
    itinerary_ = itinerary;
}

const TER_PathfindRequest::Itinerary& TER_PathfindRequest::GetItinerary() const
{
    return itinerary_;
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

std::size_t TER_PathfindRequest::GetCallerId() const
{
    return callerId_;
}
