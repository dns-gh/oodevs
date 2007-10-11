// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDirection.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterDirection::ActionParameterDirection( const kernel::OrderParameter& parameter, int value )
    : ActionParameter< int >( parameter, value )
{
    const float angle = GetValue() * 3.14f / 180.f;
    direction_ = geometry::Vector2f( sin( angle ), cos( angle ) );
}

namespace
{
    int ReadValue( xml::xistream& xis )
    {
        int value;
        xis >> xml::attribute( "value", value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterDirection::ActionParameterDirection( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< int >( parameter, ReadValue( xis ) )
{
    const float angle = GetValue() * 3.14f / 180.f;
    direction_ = geometry::Vector2f( sin( angle ), cos( angle ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterDirection::~ActionParameterDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterDirection::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 3.f );
        tools.DrawArrow( where, where + direction_ * 1000.f );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void ActionParameterDirection::Serialize( xml::xostream& xos ) const
{
    ActionParameter< int >::Serialize( xos );
    xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterDirection::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_heading;
    asn.value.u.heading = GetValue();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDirection::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterDirection::CommitTo( ASN1T_OrderContext& asn ) const
{
    asn.direction_dangereuse = GetValue();
}
