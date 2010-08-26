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
#include "dispatcher/Agent.h"
#include "dispatcher/AgentOrder.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/SimulationSenders.h"
#include <directia/Brain.h>

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentManipulator constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
AgentManipulator::AgentManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::Agent& agent, dispatcher::SimulationPublisher_ABC& publisher )
    : converter_( converter )
    , agent_    ( agent )
    , publisher_( publisher )
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
    brain.RegisterFunction( "IsMounted",           &AgentManipulator::IsMounted );
    brain.RegisterFunction( "GetForceRatio",       &AgentManipulator::GetForceRatio );
    brain.RegisterFunction( "GetMission",          &AgentManipulator::GetMission );
    brain.RegisterFunction( "Teleport",            &AgentManipulator::Teleport );
    brain.RegisterFunction( "RecoverAll",          &AgentManipulator::RecoverAll );
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
    return agent_.GetName().ascii();
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetTeam
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
std::string AgentManipulator::GetTeam() const
{
    return std::string( agent_.GetSuperior().GetTeam().GetName() );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetPosition
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
Position AgentManipulator::GetPosition() const
{
    // $$$$ _RC_ SBO 2010-05-27: reverse position_ lat/long
    const geometry::Point2d pos( agent_.GetPosition().Y(), agent_.GetPosition().X() );
    return ToPosition( converter_.ConvertFromGeo( pos ) );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetOperationalState
// Created: SBO 2008-07-23
// -----------------------------------------------------------------------------
unsigned int AgentManipulator::GetOperationalState() const
{
    return agent_.GetOperationalStateValue();
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::IsMounted
// Created: SBO 2010-06-25
// -----------------------------------------------------------------------------
bool AgentManipulator::IsMounted() const
{
    return agent_.IsMounted();
}

namespace
{
    std::string ResolveForceRatio( const MsgsSimToClient::ForceRatio::Value& ratio )
    {
        switch( ratio )
        {
        case MsgsSimToClient::ForceRatio::neutre:      return "neutral";
        case MsgsSimToClient::ForceRatio::defavorable: return "low";
        case MsgsSimToClient::ForceRatio::favorable:   return "high";
        }
        return "none";
    }
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetForceRatio
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
std::string AgentManipulator::GetForceRatio() const
{
    return ResolveForceRatio( agent_.GetForceRatio() );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetMission
// Created: SBO 2009-10-05
// -----------------------------------------------------------------------------
unsigned int AgentManipulator::GetMission() const
{
    const dispatcher::Order_ABC* order = agent_.GetOrder();
    return order ? order->GetId() : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::Teleport
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
void AgentManipulator::Teleport( const dispatcher::Position& position )
{
    simulation::UnitMagicAction message;
    message().set_oid( agent_.GetId() );
    message().set_type( MsgsClientToSim::MsgUnitMagicAction_Type_move_to );
    Common::MsgPoint point;
    Common::MsgMissionParameter& parameter = *message().mutable_parametres()->add_elem();
    parameter.set_null_value( false );
    Common::MsgLocation& location = *parameter.mutable_value()->mutable_point()->mutable_location();
    location.set_type( Common::MsgLocation::point );
    converter_.ConvertToGeo( ToPoint( position ), *location.mutable_coordinates()->add_elem() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::RecoverAll
// Created: SBO 2009-09-29
// -----------------------------------------------------------------------------
void AgentManipulator::RecoverAll()
{
    simulation::UnitMagicAction message;
    message().set_oid( agent_.GetId() );
    message().set_type( MsgsClientToSim::MsgUnitMagicAction_Type_recover_all );
    message().mutable_parametres();
    message.Send( publisher_ );
}
