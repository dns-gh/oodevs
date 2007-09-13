// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
#include "ASN_Messages.h"
#include "Tools.h"

using namespace kernel;

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
void AutomatDecisions::DoUpdate( const ASN1T_MsgAutomatAttributes& message )
{
    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomatMode::embraye );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const ASN1T_MsgAutomatOrder& message )
{
    const Resolver_ABC< Mission >& resolver = model_;
    current_ = resolver.Find( message.mission );
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
// Name: Iterator< const Mission& > AutomatDecisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Iterator< const Mission& > AutomatDecisions::GetMissions() const
{
    const Resolver_ABC< Mission >& resolver = model_;
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Iterator< const FragOrder& > AutomatDecisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Iterator< const FragOrder& > AutomatDecisions::GetFragOrders() const
{
    const Resolver_ABC< FragOrder >& resolver = model_;
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
    ASN_MsgSetAutomatMode asnMsg;
    asnMsg().oid  = agent_.GetId();
    asnMsg().mode = EnumAutomatMode::embraye;
    asnMsg.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Disengage() const
{
    ASN_MsgSetAutomatMode asnMsg;
    asnMsg().oid  = agent_.GetId();
    asnMsg().mode = EnumAutomatMode::debraye;
    asnMsg.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AutomatDecisions::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Decisions", "Automat mission:" ), current_ );
}

