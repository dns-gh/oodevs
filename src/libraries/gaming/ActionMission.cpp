// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionMission.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::ActionMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered /* = true */ )
    : Action_ABC( controller, mission, entity )
    , controller_( controller )
    , registered_( registered )
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionMission destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::~ActionMission()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionMission::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionMission::Serialize( xml::xostream& xos ) const
{
    xos << start( "mission" );
    Action_ABC::Serialize( xos );
    xos << end();
}
