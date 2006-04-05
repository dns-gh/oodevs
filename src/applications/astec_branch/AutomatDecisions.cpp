// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AutomatDecisions.h"
#include "Agent.h"
#include "DecisionalModel.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( Controller& controller, const Agent& agent )
    : controller_( controller )
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
    next_ = & agent_.GetAutomatDecisionalModel()->Resolver< Mission >::Get( message.mission.t );
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
    if( ! agent_.GetAutomatDecisionalModel() )
        throw std::runtime_error( "AutomatDecisions attached on agent" );
    return agent_.GetAutomatDecisionalModel()->Resolver< Mission >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Iterator< const FragOrder& > AutomatDecisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Iterator< const FragOrder& > AutomatDecisions::GetFragOrders() const
{
    if( ! agent_.GetAutomatDecisionalModel() )
        throw std::runtime_error( "AutomatDecisions attached on agent" );
    return agent_.GetAutomatDecisionalModel()->Resolver< FragOrder >::CreateIterator();
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
const Agent& AutomatDecisions::GetAgent() const
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
   
