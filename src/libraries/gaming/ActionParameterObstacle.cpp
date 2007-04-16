// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacle.h"
#include "clients_kernel/ObjectType.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::ActionParameterObstacle( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_MissionGenObject& asn )
    : ActionParameter< QString >( name )
    , LocationPositions( converter )
    , type_( types.Get( asn.type ) )
{
    // $$$$ SBO 2007-04-16: TODO: Handle object parameters
    Update( asn.position );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacle::~ActionParameterObstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacle::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterObstacle::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    LocationPositions::Draw( where, viewport, tools );
    type_.Draw( GetPosition(), viewport, tools );
}
