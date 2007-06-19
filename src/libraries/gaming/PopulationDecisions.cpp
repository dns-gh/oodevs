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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationDecisions constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
PopulationDecisions::PopulationDecisions( kernel::Controller& controller, const Population_ABC& popu )
    : controller_( controller )
    , popu_( popu )
    , current_( 0 )
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
// Name: PopulationDecisions::DoUpdate
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void PopulationDecisions::DoUpdate( const ASN1T_MsgPopulationOrder& message )
{
    const Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    current_ = resolver.Find( message.mission );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Iterator< const Mission& > PopulationDecisions::GetMissions
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Iterator< const Mission& > PopulationDecisions::GetMissions() const
{
    const Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::GetFragOrders
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
kernel::Iterator< const kernel::FragOrder& > PopulationDecisions::GetFragOrders() const
{
    const Resolver_ABC< FragOrder >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::GetCurrentMission
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
const kernel::Mission* PopulationDecisions::GetCurrentMission() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::GetDecisionalModel
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
const kernel::DecisionalModel& PopulationDecisions::GetDecisionalModel() const
{
    return popu_.GetType().GetDecisionalModel();
}
