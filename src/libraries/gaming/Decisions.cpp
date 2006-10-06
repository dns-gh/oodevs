// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Decisions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/AgentType.h"
#include "statusicons.h"
#include "Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "AutomatDecisions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Decisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Decisions::Decisions( Controller& controller, const Agent_ABC& agent )
    : controller_( controller )
    , agent_( agent )
    , lastOrderId_( unsigned( -1 ) )
    , current_( 0 )
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
// Name: Decisions::IsEmbraye
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
bool Decisions::IsEmbraye() const
{
    // $$$$ AGE 2006-10-06: 
    return agent_.GetAutomat().Get< AutomatDecisions >().IsEmbraye();
}

// -----------------------------------------------------------------------------
// Name: Decisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const ASN1T_MsgPionOrder& message )
{
    lastOrderId_ = message.order_id;
    const Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    current_ = & resolver.Get( message.mission.t );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Decisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const ASN1T_MsgPionOrderAck& message )
{
    if( message.error_code && message.order_id == lastOrderId_ )
    {
        current_ = 0;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Iterator< Mission > Decisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Iterator< const Mission& > Decisions::GetMissions() const
{
    const Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Iterator< const FragOrder& > Decisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Iterator< const FragOrder& > Decisions::GetFragOrders() const
{
    const Resolver_ABC< FragOrder >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetAgent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent_ABC& Decisions::GetAgent() const
{
    return agent_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetCurrentMission
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
const Mission* Decisions::GetCurrentMission() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
// $$$$ AGE 2006-10-06: move to AutomatDecisions
//void Decisions::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
//{
//    if( bEmbraye_ && viewport.IsInside( where ) )
//        tools.DrawIcon( xpm_cadenas, where, 150.f );
//}

// -----------------------------------------------------------------------------
// Name: Decisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Decisions::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Decisions", "Mission" ), current_ );
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetDecisionalModel
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const DecisionalModel& Decisions::GetDecisionalModel() const
{
    return agent_.GetType().GetDecisionalModel();
}
