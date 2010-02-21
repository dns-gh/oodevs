// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AutomatMission.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/publisher_ABC.h"
#include "protocol/simulationsenders.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace Common;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::AutomatMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, bool registered /*= true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::AutomatMission( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< MissionType >& missions, const Entity_ABC& entity )
    : Mission( xis, controller, missions, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::~AutomatMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void AutomatMission::Publish( Publisher_ABC& publisher ) const
{
    simulation::AutomatOrder message;
    message().set_oid ( GetEntity().GetId());
    message().set_mission ( GetType().GetId());
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher );
    Clean( *message().mutable_parametres() );
}

// -----------------------------------------------------------------------------
// Name: AutomatMission::Accept
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void AutomatMission::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
