// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Direction.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

#include <windows.h>
#include <gl/gl.h>

using namespace actions;
using namespace parameters;

namespace
{
    geometry::Vector2f ComputeDirection( int value )
    {
        const float angle = value * 3.14f / 180.f;
        return geometry::Vector2f( sin( angle ), cos( angle ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Direction constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Direction::Direction( const kernel::OrderParameter& parameter, int value )
    : Parameter< int >( parameter, value )
{
    direction_ = ComputeDirection( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Direction constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Direction::Direction( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : Parameter< int >( parameter, xml::attribute< int >( xis, "value" ) )
{
    direction_ = ComputeDirection( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Direction destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
Direction::~Direction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Direction::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void Direction::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
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
// Name: Direction::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void Direction::Serialize( xml::xostream& xos ) const
{
    Parameter< int >::Serialize( xos );
    xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Direction::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Direction::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_heading;
    if( IsSet() )
        asn.value.u.heading = GetValue();
}

// -----------------------------------------------------------------------------
// Name: Direction::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool Direction::IsSet() const
{
    return !direction_.IsNull();
}
