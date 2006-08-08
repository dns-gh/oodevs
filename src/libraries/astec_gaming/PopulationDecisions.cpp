// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "PopulationDecisions.h"
#include "Population_ABC.h"
#include "astec_kernel/PopulationType.h"
#include "astec_kernel/DecisionalModel.h"

// -----------------------------------------------------------------------------
// Name: PopulationDecisions constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
PopulationDecisions::PopulationDecisions( const Population_ABC& popu )
    : popu_( popu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions destructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
PopulationDecisions::~PopulationDecisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Iterator< const Mission& > PopulationDecisions::GetMissions
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Iterator< const Mission& > PopulationDecisions::GetMissions() const
{
    return popu_.GetType().GetDecisionalModel().Resolver< Mission >::CreateIterator();
}
