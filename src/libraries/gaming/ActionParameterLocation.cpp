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
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameter_ABC( name )
    , Location( converter, location )
    , parameter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameter_ABC( parameter.GetName() )
    , Location( converter, location )
    , parameter_( &parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : ActionParameter_ABC( name )
    , Location( converter, asn )
    , parameter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : ActionParameter_ABC( parameter.GetName() )
    , Location( converter, asn )
    , parameter_( &parameter )
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
void ActionParameterLocation::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    Location::Draw( where, viewport, tools );
    ActionParameter_ABC::Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::DisplayInToolTip
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterLocation::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    if( parameter_ )
    {
        displayer.Display( "", GetName() );
        ActionParameter_ABC::DisplayInToolTip( displayer );
    }
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
    ActionParameter_ABC::Serialize( xos );
    if( parameter_ )
        xos << attribute( "type", parameter_->GetType() );
    Location::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::IsContext
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
bool ActionParameterLocation::IsContext() const
{
    if( parameter_ )
        return parameter_->IsContext();
    return false;
}
