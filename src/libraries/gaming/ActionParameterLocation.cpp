// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLocation.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : ActionParameter< QString >( name, false )
    , LocationPositions( converter )
{
    Update( asn );
    SetValue( tools::ToString( asn.type ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity )
    : ActionParameter< QString >( name, false )
    , LocationPositions( converter )
{
    Update( asn, entity.Get< kernel::Positions >().GetPosition() );
    SetValue( tools::ToString( asn.type ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation destructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
ActionParameterLocation::~ActionParameterLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Draw
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionParameterLocation::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    LocationPositions::Draw( where, viewport, tools );
    if( viewport.IsVisible( GetPosition() ) )
    {
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 0, 0, 0 );
            tools.Print( GetName().ascii(), GetPosition() );
        glPopAttrib();
    }
}
