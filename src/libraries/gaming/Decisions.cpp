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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Viewport_ABC.h"
#include "Tools.h"
#include "AutomatDecisions.h"
#include "statusicons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Decisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Decisions::Decisions( Controller& controller, const Agent_ABC& agent )
    : controller_( controller )
    , agent_( agent )
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
    return agent_.Get< CommunicationHierarchies >().GetUp().Get< AutomatDecisions >().IsEmbraye();
}

// -----------------------------------------------------------------------------
// Name: Decisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const ASN1T_MsgUnitOrder& message )
{
    const Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    current_ = resolver.Find( message.mission );
    controller_.Update( *this );
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
void Decisions::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( IsEmbraye() && viewport.IsVisible( where ) )
        tools.DrawIcon( xpm_cadenas, where, 150.f );
}

// -----------------------------------------------------------------------------
// Name: Decisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Decisions::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Decisions", "Mission:" ), current_ );
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetDecisionalModel
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const DecisionalModel& Decisions::GetDecisionalModel() const
{
    return agent_.GetType().GetDecisionalModel();
}
