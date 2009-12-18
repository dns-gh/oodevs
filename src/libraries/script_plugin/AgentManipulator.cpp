// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AgentManipulator.h"
#include "directia/Brain.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentManipulator constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
AgentManipulator::AgentManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::Agent& agent )
    : converter_( converter )
    , agent_    ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator destructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
AgentManipulator::~AgentManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::Registrar::RegisterIn
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void AgentManipulator::Registrar::RegisterIn( directia::Brain& brain )
{
    brain.RegisterFunction( "GetIdentifier",       &AgentManipulator::GetIdentifier );
    brain.RegisterFunction( "GetName",             &AgentManipulator::GetName       );
    brain.RegisterFunction( "GetTeam",             &AgentManipulator::GetTeam       );
    brain.RegisterFunction( "GetPosition",         &AgentManipulator::GetPosition   );
    brain.RegisterFunction( "GetOperationalState", &AgentManipulator::GetOperationalState );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetIdentifier
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
unsigned int AgentManipulator::GetIdentifier() const
{
    return agent_.GetId();
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetName
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
std::string AgentManipulator::GetName() const
{
    return agent_.name_;
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetTeam
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
std::string AgentManipulator::GetTeam() const
{
    return std::string( agent_.automat_->team_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetPosition
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
Position AgentManipulator::GetPosition() const
{
    return ToPosition( converter_.ConvertToXY( agent_.position_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetOperationalState
// Created: SBO 2008-07-23
// -----------------------------------------------------------------------------
unsigned int AgentManipulator::GetOperationalState() const
{
    return agent_.nOperationalState_;
}
