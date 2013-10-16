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
#include <directia/brain/Brain.h>

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
void AgentManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier",       &AgentManipulator::GetIdentifier );
    brain.Register( "GetName",             &AgentManipulator::GetName       );
    brain.Register( "GetTeam",             &AgentManipulator::GetTeam       );
    brain.Register( "GetPosition",         &AgentManipulator::GetPosition   );
    brain.Register( "GetOperationalState", &AgentManipulator::GetOperationalState );
    brain.Register( "GetForceRatio",       &AgentManipulator::GetForceRatio );
    brain.Register( "GetMission",          &AgentManipulator::GetMission );

    brain.Register( "Teleport",   &AgentManipulator::Teleport );
    brain.Register( "RecoverAll", &AgentManipulator::RecoverAll );
    brain.Register( "Wound", &AgentManipulator::Wound );
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
    return agent_.GetName().toStdString();
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::GetTeam
// Created: AGE 2008-06-18
// -----------------------------------------------------------------------------
std::string AgentManipulator::GetTeam() const
{
    return agent_.GetSuperior().GetTeam().GetName().toStdString();
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

namespace
{
    std::string ResolveForceRatio( const sword::ForceRatio::Value& ratio )
    {
        switch( ratio )
        {
        case sword::ForceRatio::neutral:     return "neutral";
        case sword::ForceRatio::favorable:   return "high";
        case sword::ForceRatio::unfavorable: return "low";
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
    message().mutable_tasker()->mutable_unit()->set_id( agent_.GetId() );
    message().set_type( sword::UnitMagicAction_Type_move_to );
    sword::Point point;
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
    location.set_type( sword::Location::point );
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
    message().mutable_tasker()->mutable_unit()->set_id( agent_.GetId() );
    message().set_type( sword::UnitMagicAction_Type_recover_all );
    message().mutable_parameters();
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: AgentManipulator::Wound
// Created: LDC 2010-07-02
// -----------------------------------------------------------------------------
void AgentManipulator::Wound( int injury, int type )
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( agent_.GetId() );
    message().set_type( sword::UnitMagicAction_Type_create_wound );

    sword::MissionParameter& paramInjury = *message().mutable_parameters()->add_elem();
    paramInjury.set_null_value( false );
    paramInjury.add_value()->set_identifier( injury );

    sword::MissionParameter& paramType = *message().mutable_parameters()->add_elem();
    paramType.set_null_value( false );
    paramType.add_value()->set_identifier( type );

    message.Send( publisher_ );
}
