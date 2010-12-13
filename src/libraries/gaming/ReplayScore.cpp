// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ReplayScore.h"
#include "ScoreDefinition.h"
#include "ScoreDefinitions.h"
#include "protocol/AarSenders.h"

// -----------------------------------------------------------------------------
// Name: ReplayScore constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
ReplayScore::ReplayScore( const sword::Indicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher )
    : Score( message, definitions, controller, publisher )
    , definition_( definitions.Get( GetName() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayScore destructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
ReplayScore::~ReplayScore()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayScore::Commit
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
std::string ReplayScore::Commit( const T_Parameters& parameters ) const
{
    return definition_.Commit( parameters );
}

// -----------------------------------------------------------------------------
// Name: ReplayScore::UpdatePlots
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void ReplayScore::UpdatePlots( const sword::Indicator& )
{
    // NOTHING
}
