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
void AutomatDecisions::DoUpdate( const ASN1T_MsgAutomateAttributes& message )
{
    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const ASN1T_MsgAutomateOrder& message )
{
    const Resolver_ABC< Mission >& resolver = model_;
    current_ = & resolver.Get( message.mission );
    controller_.Update( *this );
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
const Automat_ABC& AutomatDecisions::GetAgent() const
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
    ASN_MsgSetAutomateMode asnMsg;
    asnMsg.GetAsnMsg().unit_id = agent_.GetId();
    asnMsg.GetAsnMsg().mode    = EnumAutomateState::embraye;
    asnMsg.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Disengage() const
{
    ASN_MsgSetAutomateMode asnMsg;
    asnMsg.GetAsnMsg().unit_id = agent_.GetId();
    asnMsg.GetAsnMsg().mode    = EnumAutomateState::debraye;
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

