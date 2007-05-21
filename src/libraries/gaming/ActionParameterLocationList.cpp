// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLocationList.h"
#include "ActionParameterLocation.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListLocalisation& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location %1" ).arg( i ), converter, asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterLocationList::ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterLocationList::ReadLocation, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocationList::~ActionParameterLocationList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocationList::ReadLocation
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocationList::ReadLocation( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterLocation( converter, xis ) );
}
