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
PopulationDecisions::PopulationDecisions( kernel::Controller& controller, const Population_ABC& popu )
    : controller_( controller )
    , popu_      ( popu )
    , current_   ( 0 )
    , decisionalModel_ ( &popu_.GetType().GetDecisionalModel() )
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
    const tools::Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    current_ = resolver.Find( message.type().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const Mission& > PopulationDecisions::GetMissions
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
tools::Iterator< const Mission& > PopulationDecisions::GetMissions() const
{
    const tools::Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::GetFragOrders
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::FragOrder& > PopulationDecisions::GetFragOrders() const
{
    const tools::Resolver_ABC< FragOrder >& resolver = GetDecisionalModel();
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
// Name: PopulationDecisions::GetAgent
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& PopulationDecisions::GetAgent() const
{
    return popu_;
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::GetDecisionalModel
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
const kernel::DecisionalModel& PopulationDecisions::GetDecisionalModel() const
{
    return *decisionalModel_;
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::ModelName
// Created: AHC 2012-01-23
// -----------------------------------------------------------------------------
std::string PopulationDecisions::ModelName() const
{
    return GetDecisionalModel().GetName();
}

// -----------------------------------------------------------------------------
// Name: PopulationDecisions::DoUpdate
// Created: AHC 2012-01-23
// -----------------------------------------------------------------------------
void PopulationDecisions::DoUpdate( const sword::CrowdUpdate& message )
{

}
