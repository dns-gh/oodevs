// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profiling.h"
#include "DIN_Types.h"

// -----------------------------------------------------------------------------
// Name: Profiling constructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
Profiling::Profiling()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiling destructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
Profiling::~Profiling()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiling::Update
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void Profiling::Update( const ProfilingValuesMessage& message )
{
    double rPerceptionTime, rDecisionTime, rActionTime, rMainLoopTime;
    message >> rPerceptionTime >> rDecisionTime >> rActionTime >> rMainLoopTime;
    perception_.push_back( rPerceptionTime );
    decision_  .push_back( rDecisionTime );
    action_    .push_back( rActionTime );
    total_     .push_back( rMainLoopTime );
}

    