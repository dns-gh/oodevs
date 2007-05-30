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
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/GlTooltip_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::ActionMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, bool registered /* = true */ )
    : Action_ABC( controller, mission, entity )
    , controller_( controller )
    , registered_( registered )
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

namespace
{
    const MissionType& ReadMission( xml::xistream& xis, const Resolver_ABC< MissionType >& missions )
    {
        unsigned int id = 0;
        xis >> attribute( "id", id );
        return missions.Get( id );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionMission::ActionMission( xml::xistream& xis, Controller& controller, const Resolver_ABC< MissionType >& missions, const Entity_ABC& entity )
    : Action_ABC( controller, ReadMission( xis, missions ), entity )
    , controller_( controller )
    , registered_( true )
{
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

// -----------------------------------------------------------------------------
// Name: ActionMission::Draw
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void ActionMission::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !tooltip_.get() )
    {
        std::auto_ptr< kernel::GlTooltip_ABC > tooltip = tools.CreateTooltip();
        tooltip_ = tooltip;
        kernel::Displayer_ABC& displayer = *tooltip_;
        displayer.Display( "", GetName() );
    }
    tooltip_->Draw( where );
    Action_ABC::Draw( where, viewport, tools );
}
