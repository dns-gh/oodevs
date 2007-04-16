// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDirection.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterDirection::ActionParameterDirection( const QString& name, const ASN1T_Direction& asn )
    : ActionParameter< float >( name )
    , direction_( sin( asn * 3.14f / 180.f ), -cos( asn * 3.14f / 180.f ) )
{
    SetValue( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterDirection::~ActionParameterDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterDirection::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 3.f );
        tools.DrawArrow( where, where + direction_ * tools.Pixels() * 100.f );
        glPopAttrib();
    }
}
