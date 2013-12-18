// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationDecisions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/DecisionalModel.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationDecisions constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
PopulationDecisions::PopulationDecisions( kernel::Controller& controller,
                                          const Population_ABC& population,
                                          const PopulationType& type )
    : Decisions( controller, population, type.GetDecisionalModel() )
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
// Name: PopulationDecisions::CanBeOrdered
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool PopulationDecisions::CanBeOrdered() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::DoUpdate
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void PopulationDecisions::DoUpdate( const sword::CrowdOrder& message )
{
    currentMission_ = model_->tools::Resolver< kernel::Mission >::Find( message.type().id() );
    controller_.Update( static_cast< gui::Decisions_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::DoUpdate
// Created: AHC 2012-01-23
// -----------------------------------------------------------------------------
void PopulationDecisions::DoUpdate( const sword::CrowdUpdate& message )
{
    if( message.has_brain_debug() )
        brainDebug_ = message.brain_debug();
}
