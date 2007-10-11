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
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( parameter )
    , Location( converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Location& asn )
    : ActionParameter< QString >( parameter )
    , Location( converter, asn )
{
    // NOTHING
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( OrderParameter( ReadName( xis ), "location", false ) )
    , Location( converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , Location( converter, xis )
{
    // NOTHING
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
void ActionParameterLocation::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    Location::Draw( where, viewport, tools );
    ActionParameter< QString >::Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::DisplayInToolTip
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterLocation::DisplayInToolTip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::GetPosition
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
geometry::Point2f ActionParameterLocation::GetPosition() const
{
    return Location::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ActionParameterLocation::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    Location::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_location;
    asn.value.u.location = new ASN1T_Location();
    Location::CommitTo( *asn.value.u.location );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::CommitTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void ActionParameterLocation::CommitTo( std::string& content ) const
{
    Location::CommitTo( content );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.location )
        Location::Clean( *asn.value.u.location );
    delete asn.value.u.location;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::CommitTo( ASN1T_Location& asn ) const
{
    Location::CommitTo( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::Clean( ASN1T_Location& asn ) const
{
    Location::Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLocation::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
