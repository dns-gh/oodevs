// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Decisions.h"
#include "Agent.h"
#include "Controller.h"
#include "DecisionalModel.h"

// -----------------------------------------------------------------------------
// Name: Decisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Decisions::Decisions( Controller& controller, const Agent& agent )
    : controller_( controller )
    , agent_( agent )
    , bEmbraye_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Decisions destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Decisions::~Decisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Decisions::DoUpdate
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Decisions::IsEmbraye
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool Decisions::IsEmbraye() const
{
    return bEmbraye_ || agent_.GetSuperior() && agent_.GetSuperior()->Get< Decisions >().bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: Iterator< Mission > Decisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Iterator< const Mission& > Decisions::GetMissions() const
{
    return agent_.GetDecisionalModel().Resolver< Mission >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetAgent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent& Decisions::GetAgent() const
{
    return agent_;
}
