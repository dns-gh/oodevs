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
ActionParameterLocation::ActionParameterLocation( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter_ABC( ReadName( xis ) )
    , Location( converter, xis )
    , parameter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLocation::ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter_ABC( parameter.GetName() )
    , Location( converter, xis )
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

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_localisation;
    asn.null_value = 0; // $$$$ SBO 2007-05-22: 
    asn.value.u.localisation = new ASN1T_Localisation();
    Location::CommitTo( *asn.value.u.localisation );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.localisation )
        Location::Clean( *asn.value.u.localisation );
    delete asn.value.u.localisation;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::CommitTo( ASN1T_Localisation& asn ) const
{
    Location::CommitTo( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLocation::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterLocation::Clean( ASN1T_Localisation& asn ) const
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
