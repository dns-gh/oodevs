// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPathList.h"
#include "ActionParameterPath.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListItineraire& asn, const kernel::Entity_ABC& entity )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterPath( tools::translate( "ActionParameter", "Route %1" ).arg( i ), converter, asn.elem[i], entity ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterPathList::ReadPath, converter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPathList::~ActionParameterPathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathList::ReadPath
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPathList::ReadPath( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    AddParameter( *new ActionParameterPath( converter, xis ) );
}
