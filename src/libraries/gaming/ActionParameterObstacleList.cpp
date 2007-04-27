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
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::ActionParameterObstacleList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_ListMissionGenObject& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterObstacle( tools::translate( "ActionParameter", "Obstacle %1" ).arg( i + 1 ), converter, types, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterObstacleList::~ActionParameterObstacleList()
{
    // NOTHING
}
