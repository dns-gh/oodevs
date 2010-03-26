// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "Tools.h"
#include "protocol/Protocol.h"
#include "protocol/SimulationSenders.h"
#include "protocol/Publisher_ABC.h"

using namespace kernel;
using namespace Common;

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( Controller& controller, Publisher_ABC& publisher, const Automat_ABC& agent )
    : controller_( controller )
    , publisher_( publisher )
    , agent_( agent )
    , model_( agent.GetType().GetDecisionalModel() )
    , bEmbraye_( false )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::~AutomatDecisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const MsgsSimToClient::MsgAutomatAttributes& message )
{
    if( message.has_etat_automate()  )
        bEmbraye_ = ( message.etat_automate() == EnumAutomatMode::embraye );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const Common::MsgAutomatOrder& message )
{
    const tools::Resolver_ABC< Mission >& resolver = model_;
    current_ = resolver.Find( message.mission() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::CanBeOrdered
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool AutomatDecisions::CanBeOrdered() const
{
    return bEmbraye_ && ! HasEngagedSuperior();
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::HasEngagedSuperior
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool AutomatDecisions::HasEngagedSuperior() const
{
    const Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    const AutomatDecisions* superiorDecisions = superior ? superior->Retrieve< AutomatDecisions >() : 0;
    return superiorDecisions && superiorDecisions->bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const Mission& > AutomatDecisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
tools::Iterator< const Mission& > AutomatDecisions::GetMissions() const
{
    const tools::Resolver_ABC< Mission >& resolver = model_;
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const FragOrder& > AutomatDecisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
tools::Iterator< const FragOrder& > AutomatDecisions::GetFragOrders() const
{
    const tools::Resolver_ABC< FragOrder >& resolver = model_;
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::GetCurrentMission
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
const Mission* AutomatDecisions::GetCurrentMission() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::GetAgent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Entity_ABC& AutomatDecisions::GetAgent() const
{
    return agent_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::IsEmbraye
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool AutomatDecisions::IsEmbraye() const
{
    return bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Engage() const
{
    simulation::SetAutomatMode message;
    message().set_oid( agent_.GetId() );
    message().set_mode( EnumAutomatMode::embraye );
    message.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Disengage() const
{
    simulation::SetAutomatMode message;
    message().set_oid( agent_.GetId() );
    message().set_mode( EnumAutomatMode::debraye );
    message.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AutomatDecisions::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Decisions", "Automat mission:" ), current_ );
}

