// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPath.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, const kernel::Entity_ABC& entity )
    : ActionParameterLocation( name, converter, location )
{
    PushFront( entity.Get< kernel::Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity )
    : ActionParameterLocation( name, converter, asn )
{
    PushFront( entity.Get< kernel::Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, const kernel::Entity_ABC& entity )
    : ActionParameterLocation( parameter, converter, location )
{
    PushFront( entity.Get< kernel::Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity )
    : ActionParameterLocation( parameter, converter, asn )
{
    PushFront( entity.Get< kernel::Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::~ActionParameterPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Draw
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ActionParameterPath::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    glEnable( GL_LINE_STIPPLE );
    glLineStipple( 1, tools.StipplePattern() );
        ActionParameterLocation::Draw( where, viewport, tools );
    glDisable( GL_LINE_STIPPLE );
}
