// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimit.h"
#include "ActionParameterLocation.h"
#include "ActionParameterVisitor_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Line& line )
    : ActionParameter< QString >( parameter )
    , location_( new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, (const ASN1T_Location&)line ) )
{
    AddParameter( *location_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( parameter )
    , location_( new ActionParameterLocation( OrderParameter( tools::translate( "ActionParameter", "Location" ), "location", false ), converter, location ) )
{
    AddParameter( *location_ );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( OrderParameter( ReadName( xis ), "limit", false, true ) )
{
    xis >> start( "parameter" );
    location_ = new ActionParameterLocation( converter, xis );
    AddParameter( *location_ );
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::~ActionParameterLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimit::CommitTo( ASN1T_Line& asn ) const
{
    location_->CommitTo( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimit::Clean( ASN1T_Line& asn ) const
{
    location_->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterLimit::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::GetPosition
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
geometry::Point2f ActionParameterLimit::GetPosition() const
{
    return location_->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::DisplayInToolTip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameterLimit::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}
