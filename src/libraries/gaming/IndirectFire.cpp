// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IndirectFire.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IndirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::IndirectFire( const sword::StartUnitFire& message, const tools::Resolver_ABC< Agent_ABC >& resolver, const CoordinateConverter_ABC& converter )
    : Fire_ABC( resolver.Get( message.firing_unit().id() ) )
    , target_( converter.ConvertToXY( message.target().position() ) )
{
    if( !message.target().has_position() )
        throw MASA_EXCEPTION( "Indirect fire on an agent..." );
}

IndirectFire::IndirectFire( const sword::StartUnitFireDetection& message, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::CoordinateConverter_ABC& converter )
    : Fire_ABC( resolver.Get( message.firer().id() ) )
    , target_( converter.ConvertToXY( message.target() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFire destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::~IndirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectFire::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void IndirectFire::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    const geometry::Point2f origin = GetOrigin().Get< Positions >().GetPosition();
    if( viewport.IsVisible( geometry::Rectangle2f( origin, target_ ) ) )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
            glLineWidth( 3.f );
            tools.DrawCurvedArrow( origin, target_, 0.2f, 10.f );
        glPopAttrib();
    }
}
