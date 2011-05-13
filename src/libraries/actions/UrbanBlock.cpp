// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanBlock.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "protocol/Protocol.h"
#include <windows.h>
#include <gl/gl.h>
#undef GetObject

using namespace kernel;
using namespace actions;
using namespace parameters;

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    std::string ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Object_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, unsigned long id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Object_ABC >( parameter, &resolver.GetUrbanObject( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Object_ABC >( OrderParameter( ReadName( xis ), "UrbanBlock", false ), &resolver.GetUrbanObject( ReadId( xis ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::UrbanBlock( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Object_ABC >( parameter, &resolver.GetUrbanObject( ReadId( xis ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
UrbanBlock::~UrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void UrbanBlock::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::Draw
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
void UrbanBlock::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    const bool selected = tools.ShouldDisplay();
    GLfloat color[4];
    glGetFloatv( GL_CURRENT_COLOR, color );
    glPushAttrib( GL_LINE_BIT );
    if( selected )
    {
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, color[3] * 0.5f );
        glLineWidth( 6.f );
        tools.DrawCross( GetPosition(), GL_CROSSSIZE );
        glPopAttrib();
    }
    glLineWidth( 2.f );
    tools.DrawCross( GetPosition(), GL_CROSSSIZE );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Entity< Object_ABC >::CommitTo( *message.mutable_value()->Add()->mutable_urbanknowledge() );
}
// -----------------------------------------------------------------------------
// Name: UrbanBlock::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void UrbanBlock::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        Entity< Object_ABC >::CommitTo( *message.mutable_urbanknowledge() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::GetPosition
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
geometry::Point2f UrbanBlock::GetPosition() const
{
    if( const kernel::UrbanPositions_ABC* positions = GetValue()->Retrieve< kernel::UrbanPositions_ABC >() )
        return positions->Barycenter();
    return Entity< Object_ABC >::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::DisplayInToolTip
// Created: LGY 2011-02-04
// -----------------------------------------------------------------------------
void UrbanBlock::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlock::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string UrbanBlock::SerializeType() const
{
    return "urbanblock";
}
