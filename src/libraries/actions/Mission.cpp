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

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Mission::Mission( const kernel::MissionType* mission, kernel::Controller& controller, bool registered /* = true */ )
    : Action_ABC( controller, mission )
    , controller_( controller )
    , registered_( registered )
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
    xos << xml::attribute( "id", GetType() ? GetType()->GetId() : 0 )
        << xml::attribute( "type", "mission" );
    Action_ABC::Serialize( xos );
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
    Action_ABC::Draw( where, viewport, tools );
}
