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
void IndirectFire::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( geometry::Rectangle2f( where, target_ ) ) )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
            glLineWidth( 3.f );
            tools.DrawCurvedArrow( where, target_, 0.2f, 10.f );
        glPopAttrib();
    }
}
