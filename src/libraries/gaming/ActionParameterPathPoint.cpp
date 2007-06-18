// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPathPoint.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint constructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterPathPoint::ActionParameterPathPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameterLocation( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterPathPoint::ActionParameterPathPoint( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : ActionParameterLocation( converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint destructor
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
ActionParameterPathPoint::~ActionParameterPathPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameterPathPoint::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
    ActionParameterLocation::DisplayInToolTip( displayer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathPoint::CommitTo( ASN1T_CoordUTM& asn ) const
{
    ASN1T_Location loc; // $$$$ SBO 2007-05-22: ugly trick
    ActionParameterLocation::CommitTo( loc );
    asn = loc.vecteur_point.elem[0];
    ActionParameterLocation::Clean( loc );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPathPoint::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPathPoint::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
