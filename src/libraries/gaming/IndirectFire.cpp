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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IndirectFire constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
IndirectFire::IndirectFire( const ASN1T_MsgStartUnitFire& message, const Resolver_ABC< Agent_ABC >& resolver, const CoordinateConverter_ABC& converter )
    : Fire_ABC( resolver.Get( message.tireur ) )
    , id_( message.oid_tir )
    , target_( converter.ConvertToXY( *message.cible.u.position ) )
{
    if( message.cible.t != T_MsgStartUnitFire_cible_position )
        throw std::runtime_error( "Indirect fire on an agent..." );
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
void IndirectFire::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( geometry::Rectangle2f( where, target_ ) ) )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
            glLineWidth( 3.f );
            tools.DrawCurvedArrow( where, target_, 0.2f, 10.f );
        glPopAttrib();

    }
}
