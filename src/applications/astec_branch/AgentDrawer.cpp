// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentDrawer.h"
#include "CoordinateConverter.h"
#include "Agent.h"
#include "Drawable_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentDrawer constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentDrawer::AgentDrawer( Agent& agent, const CoordinateConverter& converter, const GlTools_ABC& tools )
    : converter_( converter )
    , tools_( tools )
    , agent_( agent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentDrawer destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentDrawer::~AgentDrawer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentDrawer::Draw
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentDrawer::Draw( const geometry::Rectangle2f& viewport )
{
    // $$$$ AGE 2006-03-16: choisir geometry ou MT_Vector2D
    if( viewport.IsInside( geometry::Point2f( position_.rX_, position_.rY_ ) ) )
        agent_.InterfaceContainer< Extension_ABC >::Apply( Drawable_ABC::Draw, position_, tools_ );
}

// -----------------------------------------------------------------------------
// Name: AgentDrawer::DoUpdate
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentDrawer::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent )
        position_ = converter_.ConvertToXY( message.position );
}
