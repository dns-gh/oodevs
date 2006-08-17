// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "AutomatDecisions.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/DecisionalModel.h"
#include "astec_kernel/Controller.h"
#include "ASN_Messages.h"
#include "astec_kernel/Displayer_ABC.h"
#include "astec_kernel/AutomatType.h"

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( Controller& controller, Publisher_ABC& publisher, const Agent_ABC& agent )
    : controller_( controller )
    , publisher_( publisher )
    , agent_( agent )
    , lastOrderId_( unsigned( -1 ) )
    , current_( 0 )
    , next_( 0 )
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
void AutomatDecisions::DoUpdate( const ASN1T_MsgUnitAttributes& message )
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
    lastOrderId_ = message.order_id;
    next_ = & GetAutomatDecisionalModel().Resolver< Mission >::Get( message.mission.t -1 );// $$$$ AGE 2006-04-19: -1 car enum != asn...
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const ASN1T_MsgAutomateOrderAck& message )
{
    if( message.order_id == lastOrderId_ )
    {
        current_ = next_;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Iterator< const Mission& > AutomatDecisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Iterator< const Mission& > AutomatDecisions::GetMissions() const
{
    return GetAutomatDecisionalModel().Resolver< Mission >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Iterator< const FragOrder& > AutomatDecisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Iterator< const FragOrder& > AutomatDecisions::GetFragOrders() const
{
    return GetAutomatDecisionalModel().Resolver< FragOrder >::CreateIterator();
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
const Agent_ABC& AutomatDecisions::GetAgent() const
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
    displayer.Display( "Mission automate", current_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::GetAutomatDecisionalModel
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const DecisionalModel& AutomatDecisions::GetAutomatDecisionalModel() const
{
    if( !agent_.GetAutomatType() )
        throw std::runtime_error( "AutomatDecisions attached to an agent" );
    return agent_.GetAutomatType()->GetDecisionalModel();
}
