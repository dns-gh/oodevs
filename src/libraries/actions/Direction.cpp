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
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

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
// Created: ABR 2011-11-16
// -----------------------------------------------------------------------------
Direction::Direction( const kernel::OrderParameter& parameter )
    : Parameter< int >( parameter )
{
        // NOTHING
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
    : Parameter< int >( parameter )
{
    if( xis.has_attribute( "value" ) )
    {
        SetValue( xis.attribute< int >( "value" ) );
        direction_ = ComputeDirection( GetValue() );
    }
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
            tools.DrawArrow( where, where + direction_ * 50.f * tools.Pixels() );
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
    if( IsSet() )
        xos << xml::attribute( "value", GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Direction::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Direction::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
       message.mutable_value()->Add()->mutable_heading()->set_heading( GetValue() );
}
// -----------------------------------------------------------------------------
// Name: Direction::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Direction::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        message.mutable_heading()->set_heading( GetValue() );
}

// -----------------------------------------------------------------------------
// Name: Direction::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Direction::SerializeType() const
{
    return "heading";
}
