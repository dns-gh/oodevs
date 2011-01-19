// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Objective.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::Objective( const sword::MissionObjective& asn )
    : location_( asn.location() )
    , schedule_( asn.time().data() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::Objective()
    : location_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objective destructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::~Objective()
{
    // NOTHING
}

 // -----------------------------------------------------------------------------
// Name: Objective::Send
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
void Objective::Send( sword::MissionObjective& asn ) const
{
    asn.mutable_time()->set_data( schedule_ );
    location_.Send( *asn.mutable_location() );
}
