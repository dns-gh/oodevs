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

// -----------------------------------------------------------------------------
// Name: ReplayScore constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
ReplayScore::ReplayScore( const ASN1T_MsgIndicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher )
    : Score( message, controller, publisher )
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
