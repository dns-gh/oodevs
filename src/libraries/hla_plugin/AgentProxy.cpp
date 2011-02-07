// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentProxy.h"
#include "clients_kernel/AgentType.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentProxy constructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::AgentProxy( dispatcher::Agent_ABC& agent )
    : dispatcher::Observer< sword::UnitAttributes >( agent )
    , dispatcher::Observer< sword::UnitEnvironmentType >( agent )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy destructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::~AgentProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetId
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
std::string AgentProxy::GetId() const
{
    return boost::lexical_cast< std::string >( agent_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetName
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
std::string AgentProxy::GetName() const
{
    return agent_.GetName().ascii();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetType
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
std::string AgentProxy::GetType() const
{
    return agent_.GetType().GetName();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetPosition
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
geometry::Point2d AgentProxy::GetPosition() const
{
    return agent_.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetAltitude
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
unsigned short AgentProxy::GetAltitude() const
{
    return agent_.GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetSpeed
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
unsigned short AgentProxy::GetSpeed() const
{
    return agent_.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetDirection
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
unsigned short AgentProxy::GetDirection() const
{
    return agent_.GetDirection();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetForce
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
const kernel::Karma& AgentProxy::GetForce() const
{
    return agent_.GetSuperior().GetTeam().GetKarma();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetEquipments
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Equipment >& AgentProxy::GetEquipments() const
{
    return agent_.Equipments();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitAttributes& attributes )
{
    dispatcher::Observable< sword::UnitAttributes >::Notify( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitEnvironmentType& attributes )
{
    dispatcher::Observable< sword::UnitEnvironmentType >::Notify( attributes );
}
