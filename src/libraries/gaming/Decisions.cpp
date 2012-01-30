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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
#include "AutomatDecisions.h"
#include "statusicons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Decisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Decisions::Decisions( Controller& controller, const Agent_ABC& agent,
        const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver)
    : controller_( controller )
    , agent_( agent )
    , modelResolver_( modelResolver )
    , current_( 0 )
    , decisionalModel_( &( agent.GetType().GetDecisionalModel() ) )
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
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const sword::UnitOrder& message )
{
    const tools::Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    current_ = resolver.Find( message.type().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Decisions::CanBeOrdered
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool Decisions::CanBeOrdered() const
{
    const AutomatDecisions& superiorDecisions = agent_.Get< TacticalHierarchies >().GetUp().Get< AutomatDecisions >();
    return !superiorDecisions.IsEmbraye();
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< Mission > Decisions::GetMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
tools::Iterator< const Mission& > Decisions::GetMissions() const
{
    const tools::Resolver_ABC< Mission >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const FragOrder& > Decisions::GetFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
tools::Iterator< const FragOrder& > Decisions::GetFragOrders() const
{
    const tools::Resolver_ABC< FragOrder >& resolver = GetDecisionalModel();
    return resolver.CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetAgent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Entity_ABC& Decisions::GetAgent() const
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
    if( !CanBeOrdered() && viewport.IsVisible( where ) )
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
    return *decisionalModel_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::DoUpdate
// Created: AHC 2012-01-11
// -----------------------------------------------------------------------------
void Decisions::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_decisonal_model() && message.decisonal_model()!=decisionalModel_->GetName() )
    {
        decisionalModel_ = &modelResolver_.Get( message.decisonal_model() );
    }
}

// -----------------------------------------------------------------------------
// Name: Decisions::ModelName
// Created: AHC 2012-01-23
// -----------------------------------------------------------------------------
std::string Decisions::ModelName() const
{
    return GetDecisionalModel().GetName();
}