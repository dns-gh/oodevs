// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacleList.h"
#include "ActionParameterObstacle.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_ListMissionGenObject& asn )
    : ActionParameter_ABC( name )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        Register( i, *new ActionParameterObstacle( "", converter, types, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::~ActionParameterObstacleList()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::Display
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "ActionParameter", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterObstacleList::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( where, viewport, tools );
}
