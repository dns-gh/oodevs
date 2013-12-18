// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( Controller& controller,
                                    const Automat_ABC& agent,
                                    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver )
    : Decisions( controller, agent, agent.GetType().GetDecisionalModel() )
    , modelResolver_( modelResolver )
    , engaged_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( xml::xistream& xis,
                                    kernel::Controller& controller,
                                    const kernel::Automat_ABC& agent,
                                    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver )
    : Decisions( controller, agent, agent.GetType().GetDecisionalModel() )
    , modelResolver_( modelResolver )
    , engaged_( xis.attribute< std::string >( "engaged", "true" ) == "true" )
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
// Name: AutomatDecisions::CanBeOrdered
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool AutomatDecisions::CanBeOrdered() const
{
    return engaged_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::IsEngaged
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool AutomatDecisions::IsEngaged() const
{
    return engaged_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::SetEngaged
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::SetEngaged( bool engaged )
{
    engaged_ = engaged;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AutomatDecisions::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AutomatDecisions", "Automat mission:" ), currentMission_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AutomatDecisions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "engaged", engaged_ ? "true" : "false" );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const sword::AutomatAttributes& message )
{
    if( message.has_mode() )
        SetEngaged( message.mode() == sword::engaged );
    if( message.has_decisional_model() && message.decisional_model() != GetModelName() )
        model_ = &modelResolver_.Get( message.decisional_model() );
    if( message.has_brain_debug() )
        brainDebug_ = message.brain_debug();
    controller_.Update( static_cast< gui::Decisions_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::DoUpdate
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::DoUpdate( const sword::AutomatOrder& message )
{
    currentMission_ = model_->tools::Resolver< kernel::Mission >::Find( message.type().id() );
    controller_.Update( static_cast< gui::Decisions_ABC& >( *this ) );
}
