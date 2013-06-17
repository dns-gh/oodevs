// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Mission.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/GlTooltip_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;

namespace
{
    const kernel::OrderType& ResolveType( xml::xistream& xis, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub )
    {
        const kernel::OrderType* type = 0;
        try
        {
            const unsigned int id = xis.attribute< unsigned int >( "id", 0 );
            type = missions.Find( id );
            if( !type )
            {
                const std::string name = xis.attribute< std::string >( "name", "" );
                throw MASA_EXCEPTION( tools::translate( "Mission", "Entity '%1' (id: %2) cannot execute mission '%3' (id: %4)" )
                                      .arg( entity.GetName() ).arg( entity.GetId() ).arg( name.c_str() ).arg( id ).toStdString() );
            }
        }
        catch( const std::exception& )
        {
            if( stub )
            {
                static const kernel::OrderType stubType;
                return stubType;
            }
            throw MASA_EXCEPTION( tools::translate( "Mission", "Entity '%1' (id: %2) received unknown mission" )
                                  .arg( entity.GetName() ).arg( entity.GetId() ).arg( "?" ).arg( "?" ).toStdString() );
        }
        return *type;
    }
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Mission::Mission( const kernel::Entity_ABC* entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered /* = true */ )
    : ActionWithTarget_ABC( controller, mission, entity )
    , controller_         ( controller )
    , registered_         ( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub )
    : ActionWithTarget_ABC( xis, controller, ResolveType( xis, missions, entity, stub ), entity )
    , controller_         ( controller )
    , registered_         ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    if( registered_ )
        controller_.Delete( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Mission::Polish
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void Mission::Polish()
{
    if( registered_ )
        controller_.Create( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Mission::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Mission::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetId() )
        << xml::attribute( "type", "mission" );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Mission::Draw
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void Mission::Draw( const geometry::Point2f& where, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !tooltip_.get() )
    {
        std::auto_ptr< gui::GlTooltip_ABC > tooltip = tools.CreateTooltip();
        tooltip_ = tooltip;
        kernel::Displayer_ABC& displayer = *tooltip_;
        displayer.Display( "", GetName() );
    }
    tooltip_->Draw( where );
    ActionWithTarget_ABC::Draw( where, viewport, tools );
}
