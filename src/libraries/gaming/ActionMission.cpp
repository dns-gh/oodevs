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
#include "Tools.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/GlTooltip_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

namespace
{
    const OrderType& ResolveType( xml::xistream& xis, const Resolver_ABC< MissionType >& missions, const Entity_ABC& entity )
    {
        const unsigned int id = xml::attribute< unsigned int >( xis, "id", 0 );
        const std::string name = xml::attribute< std::string >( xis, "name", "" );
        const OrderType* type = missions.Find( id );
        if( !type )
            throw std::exception( tools::translate( "ActionMission", "Entity '%1' (id: %2) cannot execute mission '%3' (id: %4)" )
                                    .arg( entity.GetName() ).arg( entity.GetId() ).arg( name.c_str() ).arg( id ) );
        return *type;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::ActionMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, bool registered /* = true */ )
    : Action_ABC( controller, mission, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionMission::ActionMission( xml::xistream& xis, Controller& controller, const Resolver_ABC< MissionType >& missions, const Entity_ABC& entity )
    : Action_ABC( xis, controller, ResolveType( xis, missions, entity ), entity )
    , controller_( controller )
    , registered_( true )
{
    // NOTHING
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
// Name: ActionMission::Polish
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void ActionMission::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionMission::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionMission::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "mission" );
    Action_ABC::Serialize( xos );
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
