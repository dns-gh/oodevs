// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentDecisions.h"
#include "statusicons.h"

#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_gui/Tools.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"

#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: AgentDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AgentDecisions::AgentDecisions( kernel::Controller& controller,
                                const kernel::Agent_ABC& agent,
                                const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver )
    : Decisions( controller, agent, agent.GetType().GetDecisionalModel() )
    , modelResolver_( modelResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AgentDecisions::~AgentDecisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions::CanBeOrdered
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool AgentDecisions::CanBeOrdered() const
{
    return !tools::IsSuperiorEngaged( entity_ );
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentDecisions::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !CanBeOrdered() && viewport.IsVisible( where ) && !entity_.IsAggregated() )
        tools.DrawIcon( xpm_cadenas, where, 150.f, gui::GlTools_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AgentDecisions::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentDecisions", "Mission:" ), currentMission_ );
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AgentDecisions::DoUpdate( const sword::UnitOrder& message )
{
    currentMission_ = model_->tools::Resolver< kernel::Mission >::Find( message.type().id() );
    controller_.Update( static_cast< gui::Decisions_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: AgentDecisions::DoUpdate
// Created: AHC 2012-01-11
// -----------------------------------------------------------------------------
void AgentDecisions::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_decisional_model() && message.decisional_model() != model_->GetName() )
        model_ = &modelResolver_.Get( message.decisional_model() );
    if( message.has_brain_debug() )
        brainDebug_ = message.brain_debug();
}
